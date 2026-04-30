// auth.ts — bearer-token preHandler.
//
// We attach the hook directly to the root Fastify instance (not via a
// plugin), because hooks added inside a plugin's encapsulated scope only
// apply to routes registered IN that plugin's children — and the routes
// we want to protect live in index.ts at the top level.
//
// /api/health is intentionally open so liveness checks work without the
// secret. Everything else under /api/* requires `Authorization: Bearer
// <AUTH_TOKEN>`.

import type { FastifyInstance } from "fastify";
import { config } from "./config.js";

export function attachBearerAuth(app: FastifyInstance): void {
  app.addHook("preHandler", async (req, reply) => {
    if (req.url.startsWith("/api/health")) return;
    const expected = `Bearer ${config.authToken}`;
    const header   = req.headers.authorization ?? "";
    if (header !== expected) {
      return reply.code(401).send({ error: "unauthorized" });
    }
  });
}
