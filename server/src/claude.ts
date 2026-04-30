// claude.ts — read Claude Code's OAuth token out of the macOS keychain
// and proxy Anthropic's Usage API.
//
// Source of truth (per CCSwitcher's reverse-engineering):
//   - Keychain entry "Claude Code-credentials" stores
//     {claudeAiOauth: {accessToken, subscriptionType, ...}}
//   - ~/.claude.json holds {oauthAccount: {emailAddress, ...}}
//   - GET https://api.anthropic.com/api/oauth/usage with bearer token
//     returns {fiveHour, sevenDay, extraUsage, ...}
//
// We read all three, shape the result for the Tab5 dashboard card, and
// memoize for 60 s so client polling is cheap.

import { execFile as execFileCb } from "node:child_process";
import { readFile } from "node:fs/promises";
import { homedir } from "node:os";
import { join } from "node:path";
import { promisify } from "node:util";

import { memo } from "./cache.js";

const execFile = promisify(execFileCb);

interface KeychainEntry {
  claudeAiOauth: {
    accessToken:       string;
    refreshToken?:     string;
    expiresAt?:        number;
    subscriptionType?: string;
    rateLimitTier?:    string;
    scopes?:           string[];
  };
}

// Anthropic returns snake_case on the wire. (CCSwitcher's report
// described camelCase because Swift's JSONDecoder
// .convertFromSnakeCase rewrote keys client-side.)
interface UsageAPIResp {
  five_hour?:   { utilization: number; resets_at: string };
  seven_day?:   { utilization: number; resets_at: string };
  extra_usage?: {
    is_enabled?:   boolean;
    monthly_limit?: number | null;
    used_credits?:  number | null;
    utilization?:   number | null;
    currency?:      string | null;
  };
}

async function readKeychainToken(): Promise<KeychainEntry> {
  // -w prints just the password (the OAuth JSON), no metadata.
  // First call may surface a macOS keychain access prompt; click
  // "Always Allow" once and subsequent calls run silently.
  const { stdout } = await execFile("security", [
    "find-generic-password",
    "-s", "Claude Code-credentials",
    "-w",
  ]);
  const text = stdout.trim();
  try {
    return JSON.parse(text) as KeychainEntry;
  } catch (e) {
    throw new Error(`keychain entry was not JSON: ${(e as Error).message}`);
  }
}

async function readAccountEmail(): Promise<string | null> {
  try {
    const path = join(homedir(), ".claude.json");
    const raw  = await readFile(path, "utf-8");
    const j    = JSON.parse(raw) as { oauthAccount?: { emailAddress?: string } };
    return j.oauthAccount?.emailAddress ?? null;
  } catch {
    return null;
  }
}

// CCSwitcher's display style: first 3 chars + "*@*." + tld.
//   joey@gmail.com → joe*@*.com
function maskEmail(email: string): string {
  const at = email.indexOf("@");
  if (at < 0) return email;
  const local  = email.slice(0, at);
  const domain = email.slice(at + 1);
  const tld    = domain.split(".").pop() ?? domain;
  const head   = local.slice(0, Math.min(3, local.length));
  return `${head}*@*.${tld}`;
}

function planDisplay(subType: string | undefined): string {
  if (!subType) return "Pro";
  const lower = subType.toLowerCase();
  return lower.charAt(0).toUpperCase() + lower.slice(1);
}

// "in 3 hr 6 min" / "Mon 06:00" — short, fits in a card header line.
function timeToReset(resetsAtIso: string): string {
  const target = new Date(resetsAtIso).getTime();
  if (!Number.isFinite(target)) return "—";
  const remaining = target - Date.now();
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

export interface ClaudeResp {
  email:       string;
  emailMasked: string;
  plan:        string;
  session:     { utilizationPct: number; resetsAt: string; resetIn: string } | null;
  weekly:      { utilizationPct: number; resetsAt: string; resetIn: string } | null;
  extraUsage:  { enabled: boolean };
  fetchedAt:   string;
}

export async function getClaude(): Promise<ClaudeResp> {
  return memo("claude", 60_000, async () => {
    const kc      = await readKeychainToken();
    const token   = kc.claudeAiOauth.accessToken;
    const subType = kc.claudeAiOauth.subscriptionType;
    if (!token) throw new Error("keychain entry missing accessToken");

    const res = await fetch("https://api.anthropic.com/api/oauth/usage", {
      headers: {
        Authorization:    `Bearer ${token}`,
        "anthropic-beta": "oauth-2025-04-20",
      },
    });
    if (!res.ok) {
      const body = await res.text().catch(() => "");
      throw new Error(`anthropic http ${res.status}: ${body.slice(0, 200)}`);
    }
    const usage = (await res.json()) as UsageAPIResp;

    const email = (await readAccountEmail()) ?? "unknown";

    return {
      email,
      emailMasked: maskEmail(email),
      plan:        planDisplay(subType),
      session: usage.five_hour
        ? {
            utilizationPct: Math.round(usage.five_hour.utilization),
            resetsAt:       usage.five_hour.resets_at,
            resetIn:        timeToReset(usage.five_hour.resets_at),
          }
        : null,
      weekly: usage.seven_day
        ? {
            utilizationPct: Math.round(usage.seven_day.utilization),
            resetsAt:       usage.seven_day.resets_at,
            resetIn:        timeToReset(usage.seven_day.resets_at),
          }
        : null,
      extraUsage: { enabled: usage.extra_usage?.is_enabled ?? false },
      fetchedAt:  new Date().toISOString(),
    };
  });
}
