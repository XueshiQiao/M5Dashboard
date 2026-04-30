// cache.ts — tiny TTL memo so repeated client polls don't hammer upstream.
//
// One in-memory map keyed by string. Each entry stores the value + an
// expiresAt timestamp; on lookup we return the value if still fresh,
// otherwise we re-fetch.

interface Entry<T> {
  value:     T;
  expiresAt: number;
}

const store = new Map<string, Entry<unknown>>();

export async function memo<T>(
  key: string,
  ttlMs: number,
  fetcher: () => Promise<T>,
): Promise<T> {
  const now = Date.now();
  const hit = store.get(key) as Entry<T> | undefined;
  if (hit && hit.expiresAt > now) return hit.value;
  try {
    const value = await fetcher();
    store.set(key, { value, expiresAt: now + ttlMs });
    return value;
  } catch (err) {
    // Stale-while-error: if we have a previously cached value, keep
    // serving it while the upstream is unavailable (e.g. Anthropic
    // 429ing the Usage API). Bump expiry by a short cooldown so we
    // don't retry the upstream on every request.
    if (hit) {
      store.set(key, { value: hit.value, expiresAt: now + 60_000 });
      return hit.value;
    }
    throw err;
  }
}
