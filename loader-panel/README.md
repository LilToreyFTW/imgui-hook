# RedEyedNinja-X Loader Panel

Next.js + Clerk auth panel for subscription key management.

## Setup

1. Create a Clerk application at https://dashboard.clerk.com
2. Copy `.env.local.example` to `.env.local`
3. Add your Clerk keys
4. Add `OWNER_KEY=REN-OWNER-7F3E9A2B4C8D1E60` (or your chosen owner key)
5. (Vercel) Add Upstash Redis from Vercel Marketplace for key storage
6. Run `npm run dev`

## API

- `POST /api/validate` – Body: `{ "key": "...", "hwid": "..." }` – Returns `{ "valid": true|false, "message"?: "..." }`
- `GET /api/ping` – Returns `{ "ok": true }` – Loader uses this for latency display
- `GET /api/key` – Get current user's key (auth required)

## Deploy to Vercel

1. Connect repo or run `npx vercel`
2. Add environment variables in Vercel dashboard:
   - Clerk keys
   - `OWNER_KEY` (your owner key)
   - Upstash Redis: `KV_REST_API_URL`, `KV_REST_API_TOKEN` (auto-injected if added from Marketplace)
3. Update `panel_url.txt` next to Loader.exe with your Vercel URL (e.g. `https://loader-panel-xxx.vercel.app`)

## HWID Binding

Keys bind to the computer's HWID on first use. A key cannot be used on a different PC unless it's the owner key.
