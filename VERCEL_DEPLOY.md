# Deploy to Vercel

The Next.js app is at the **repo root** – no Root Directory setting needed.

1. Go to [vercel.com](https://vercel.com) → **Add New** → **Project**
2. Import `LilToreyFTW/imgui-hook` from GitHub
3. Add **Environment Variables**:
   - `NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY`
   - `CLERK_SECRET_KEY`
   - `OWNER_KEY` (your owner key)
   - Optional: `KV_REST_API_URL`, `KV_REST_API_TOKEN` (Upstash Redis)
4. Click **Deploy**

## Update Loader

After deploy, edit `panel_url.txt` next to Loader.exe with your Vercel URL (e.g. `https://imgui-hook.vercel.app`).
