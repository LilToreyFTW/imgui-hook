# Deploy to Vercel

The Next.js app is at the **repo root** – no Root Directory setting needed.

1. Go to [vercel.com](https://vercel.com) → **Add New** → **Project**
2. Import `LilToreyFTW/imgui-hook` from GitHub
3. Add **Environment Variables**:
   - `NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY` – from [Clerk Dashboard](https://dashboard.clerk.com)
   - `CLERK_SECRET_KEY` – from Clerk Dashboard
   - `OWNER_KEY=REN-OWNER-7F3E9A2B4C8D1E60` – use in Loader for sign in (bypasses HWID)
   - `ADMIN_USER_IDS=user_37mP4npI7kykjmfLG1UzKcV6q9m` – Clerk user IDs who can access /admin
   - Optional: `KV_REST_API_URL`, `KV_REST_API_TOKEN` (Upstash Redis)
4. Click **Deploy**

## Local dev

Copy `.env.local.example` to `.env.local` and fill in Clerk keys. `OWNER_KEY` and `ADMIN_USER_IDS` are already set.

## Update Loader

After deploy, edit `panel_url.txt` next to Loader.exe with your Vercel URL (e.g. `https://imgui-hook.vercel.app`).
