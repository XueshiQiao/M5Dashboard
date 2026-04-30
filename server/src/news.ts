// news.ts — AI-flavoured news for the dashboard's inbox card.
//
// Source: Kagi News (`https://news.kagi.com/{slug}.xml`). Kagi News is
// a curated, multi-source feed — every item is a short summary with
// links to multiple original outlets. Far higher signal/noise than HN
// for this card.
//
// We parse the RSS XML with regex (the feed is well-formed and we only
// pull out title + link) so there's no XML-parser dep. Memo'd 30 min.

import { config } from "./config.js";
import { memo } from "./cache.js";

export interface NewsItem {
  title:    string;
  url:      string;
  source:   string;        // "Kagi"
  // Kept for forward-compat with the old shape, both 0 with this source.
  score:    number;
  comments: number;
  createdAt: string;       // ISO of feed pubDate / or empty
}

export interface NewsResp {
  items:     NewsItem[];
  fetchedAt: string;
}

// XML entity unescape — RSS titles can contain &amp; / &#x27; etc.
function unescape(s: string): string {
  return s
    .replace(/&lt;/g,   "<")
    .replace(/&gt;/g,   ">")
    .replace(/&quot;/g, '"')
    .replace(/&#x27;/g, "'")
    .replace(/&#39;/g,  "'")
    .replace(/&apos;/g, "'")
    .replace(/&amp;/g,  "&")
    .replace(/&#(\d+);/g, (_, n) => String.fromCodePoint(Number(n)));
}

function pickTag(item: string, tag: string): string {
  // Match either <tag>plain text</tag> or <tag><![CDATA[…]]></tag>.
  const re = new RegExp(`<${tag}[^>]*>(?:<!\\[CDATA\\[)?([\\s\\S]*?)(?:\\]\\]>)?</${tag}>`, "i");
  const m  = re.exec(item);
  return m ? unescape(m[1]!.trim()) : "";
}

function parseRss(xml: string): NewsItem[] {
  const out: NewsItem[] = [];
  const itemRe = /<item\b[^>]*>([\s\S]*?)<\/item>/g;
  let m: RegExpExecArray | null;
  while ((m = itemRe.exec(xml)) !== null) {
    const block   = m[1]!;
    const title   = pickTag(block, "title");
    if (!title) continue;
    const link    = pickTag(block, "link");
    const pubDate = pickTag(block, "pubDate");
    out.push({
      title,
      url:       link,
      source:    "Kagi",
      score:     0,
      comments:  0,
      createdAt: pubDate ? new Date(pubDate).toISOString() : "",
    });
  }
  return out;
}

export async function getNews(): Promise<NewsResp> {
  return memo("news", 30 * 60 * 1000, async () => {
    const url = `https://news.kagi.com/${config.news.feedSlug}.xml`;
    const ctl = AbortSignal.timeout(8000);
    const res = await fetch(url, {
      signal: ctl,
      // Kagi News serves the feed without auth, but a polite UA helps
      // any logging on their side and keeps generic-bot heuristics happy.
      headers: { "User-Agent": "m5dashboard-server (+https://github.com/local/m5dashboard)" },
    });
    if (!res.ok) throw new Error(`kagi news http ${res.status} for ${url}`);
    const xml   = await res.text();
    const items = parseRss(xml);
    return { items, fetchedAt: new Date().toISOString() };
  });
}
