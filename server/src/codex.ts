// codex.ts — read ChatGPT-tier rate limits via the Codex CLI's app-server.
//
// Codex (the OpenAI coding agent CLI) ships an `app-server` subcommand
// that speaks newline-delimited JSON-RPC and is the same channel its
// own TUI uses for status reads. We spawn it once and reuse the
// connection. The OAuth tokens stay inside the codex binary; we never
// see them.
//
// One useful method: `account/rateLimits/read` →
//   { rateLimits: {
//       primary:   { usedPercent, windowDurationMins, resetsAt },
//       secondary: { usedPercent, windowDurationMins, resetsAt },
//       credits:   { hasCredits, unlimited, balance },
//       planType:  "plus" | "pro" | "team" | ...,
//     },
//     rateLimitsByLimitId: { [limitId]: <same shape> }
//   }
//
// Ref: `codex app-server generate-ts` (TS protocol bindings).

import { spawn, type ChildProcessWithoutNullStreams } from "node:child_process";
import { readFile } from "node:fs/promises";
import { homedir } from "node:os";
import { join } from "node:path";

import { memo } from "./cache.js";

interface RateLimitBucket {
  usedPercent:        number;
  windowDurationMins: number;
  resetsAt:           number;   // unix seconds
}

interface RateLimitsBlock {
  limitId:               string;
  limitName?:            string | null;
  primary:               RateLimitBucket;
  secondary?:            RateLimitBucket | null;
  credits?:              { hasCredits: boolean; unlimited: boolean; balance: string } | null;
  planType:              string;
  rateLimitReachedType?: string | null;
}

interface RateLimitsResp {
  rateLimits:           RateLimitsBlock;
  rateLimitsByLimitId?: Record<string, RateLimitsBlock>;
}

// --- long-lived JSON-RPC channel to `codex app-server` ---------------------

interface JsonRpcReq  { jsonrpc: "2.0"; id: number; method: string; params: unknown }
interface JsonRpcResp { jsonrpc?: "2.0"; id: number; result?: unknown; error?: { code: number; message: string } }

let proc:        ChildProcessWithoutNullStreams | null = null;
let initialized: Promise<void> | null = null;
let nextId       = 1;
const waiters    = new Map<number, (msg: JsonRpcResp) => void>();
let stdoutBuf    = "";

function startProcess(): ChildProcessWithoutNullStreams {
  const p = spawn("codex", ["app-server"], { stdio: ["pipe", "pipe", "pipe"] });
  p.stdout.setEncoding("utf-8");
  p.stdout.on("data", (chunk: string) => {
    stdoutBuf += chunk;
    while (true) {
      const nl = stdoutBuf.indexOf("\n");
      if (nl < 0) return;
      const line = stdoutBuf.slice(0, nl).trim();
      stdoutBuf  = stdoutBuf.slice(nl + 1);
      if (!line) continue;
      try {
        const msg = JSON.parse(line) as JsonRpcResp;
        if (msg.id != null && waiters.has(msg.id)) {
          waiters.get(msg.id)!(msg);
          waiters.delete(msg.id);
        }
      } catch {
        // codex sometimes writes non-JSON tracing on stdout; ignore those frames.
      }
    }
  });
  p.on("exit", (code, sig) => {
    console.warn(`[codex] app-server exited (code=${code}, sig=${sig}); will respawn on next request`);
    proc        = null;
    initialized = null;
    waiters.clear();
    stdoutBuf   = "";
  });
  // codex's own logs go to stderr — keep them out of our normal output.
  p.stderr.on("data", () => { /* swallow */ });
  return p;
}

async function ensureProcess(): Promise<ChildProcessWithoutNullStreams> {
  if (proc) return proc;
  proc        = startProcess();
  initialized = (async () => {
    await rpc("initialize", {
      clientInfo:   { name: "m5dashboard-server", version: "0.1.0" },
      capabilities: {},
    });
  })();
  await initialized;
  return proc;
}

function rpc(method: string, params: unknown, timeoutMs = 8000): Promise<unknown> {
  return new Promise(async (resolve, reject) => {
    const p   = proc!;
    const id  = nextId++;
    const req: JsonRpcReq = { jsonrpc: "2.0", id, method, params };
    const t   = setTimeout(() => {
      waiters.delete(id);
      reject(new Error(`codex rpc ${method} timed out`));
    }, timeoutMs);
    waiters.set(id, (msg) => {
      clearTimeout(t);
      if (msg.error) reject(new Error(`codex rpc ${method}: ${msg.error.message}`));
      else           resolve(msg.result);
    });
    p.stdin.write(JSON.stringify(req) + "\n");
  });
}

async function call<T>(method: string, params: unknown = {}): Promise<T> {
  await ensureProcess();
  return (await rpc(method, params)) as T;
}

// --- email / plan extraction from auth.json --------------------------------

interface AuthJson {
  auth_mode?: string;
  tokens?: {
    id_token?:     string;
    access_token?: string;
    refresh_token?: string;
    account_id?:   string;
  };
}

interface IdTokenPayload {
  email?: string;
  "https://api.openai.com/auth"?: {
    chatgpt_plan_type?: string;
    chatgpt_user_id?:   string;
  };
}

function decodeJwtPayload(jwt: string): IdTokenPayload {
  const parts = jwt.split(".");
  if (parts.length < 2) return {};
  let s = parts[1]!.replace(/-/g, "+").replace(/_/g, "/");
  while (s.length % 4) s += "=";
  return JSON.parse(Buffer.from(s, "base64").toString("utf-8")) as IdTokenPayload;
}

async function readAuth(): Promise<{ email: string | null; plan: string | null }> {
  try {
    const path = join(homedir(), ".codex", "auth.json");
    const raw  = await readFile(path, "utf-8");
    const j    = JSON.parse(raw) as AuthJson;
    const tok  = j.tokens?.id_token;
    if (!tok) return { email: null, plan: null };
    const claims = decodeJwtPayload(tok);
    const oa     = claims["https://api.openai.com/auth"] ?? {};
    return {
      email: claims.email ?? null,
      plan:  oa.chatgpt_plan_type ?? null,
    };
  } catch {
    return { email: null, plan: null };
  }
}

// CCSwitcher-style email mask: first 3 + "*@*." + tld.
function maskEmail(email: string): string {
  const at = email.indexOf("@");
  if (at < 0) return email;
  const local  = email.slice(0, at);
  const domain = email.slice(at + 1);
  const tld    = domain.split(".").pop() ?? domain;
  const head   = local.slice(0, Math.min(3, local.length));
  return `${head}*@*.${tld}`;
}

function planDisplay(planType: string | null): string {
  if (!planType) return "ChatGPT";
  return planType.charAt(0).toUpperCase() + planType.slice(1).toLowerCase();
}

// "3h 6m to reset" / "Mon 06:00 reset" — same shape as claude.ts produces.
function timeToReset(epochSec: number): string {
  const target = epochSec * 1000;
  const remaining = target - Date.now();
  if (!Number.isFinite(target) || target <= 0) return "—";
  if (remaining <= 0) return "resetting";
  const totalMin = Math.floor(remaining / 60_000);
  if (totalMin < 60) return `${totalMin}m to reset`;
  const h = Math.floor(totalMin / 60);
  const m = totalMin % 60;
  if (h < 24) return `${h}h ${m}m to reset`;
  const d   = new Date(target);
  const dow = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"][d.getDay()] ?? "";
  const hh  = String(d.getHours()).padStart(2, "0");
  const mm  = String(d.getMinutes()).padStart(2, "0");
  return `${dow} ${hh}:${mm} reset`;
}

// --- public ----------------------------------------------------------------

export interface CodexResp {
  email:       string;
  emailMasked: string;
  plan:        string;
  session:     { utilizationPct: number; resetsAt: string; resetIn: string } | null;
  weekly:      { utilizationPct: number; resetsAt: string; resetIn: string } | null;
  extraUsage:  { enabled: boolean };
  fetchedAt:   string;
}

export async function getCodex(): Promise<CodexResp> {
  return memo("codex", 60_000, async () => {
    const [rl, auth] = await Promise.all([
      call<RateLimitsResp>("account/rateLimits/read", {}),
      readAuth(),
    ]);

    const blk = rl.rateLimits;
    const session = blk.primary
      ? {
          utilizationPct: Math.round(blk.primary.usedPercent),
          resetsAt:       new Date(blk.primary.resetsAt * 1000).toISOString(),
          resetIn:        timeToReset(blk.primary.resetsAt),
        }
      : null;
    const weekly = blk.secondary
      ? {
          utilizationPct: Math.round(blk.secondary.usedPercent),
          resetsAt:       new Date(blk.secondary.resetsAt * 1000).toISOString(),
          resetIn:        timeToReset(blk.secondary.resetsAt),
        }
      : null;

    const email = auth.email ?? "unknown";

    return {
      email,
      emailMasked: maskEmail(email),
      // Prefer the plan from the rateLimits response (live), fall back to
      // the JWT claim, fall back to "ChatGPT".
      plan:        planDisplay(blk.planType ?? auth.plan),
      session,
      weekly,
      extraUsage:  { enabled: blk.credits?.hasCredits ?? false },
      fetchedAt:   new Date().toISOString(),
    };
  });
}
