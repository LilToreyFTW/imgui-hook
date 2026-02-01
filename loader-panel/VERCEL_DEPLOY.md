# Deploy to Vercel

## Via GitHub (recommended)

1. Go to [vercel.com](https://vercel.com) and sign in
2. Click **Add New** → **Project**
3. Import `LilToreyFTW/imgui-hook` from GitHub
4. **Root Directory**: Set to `loader-panel` (click Edit, type `loader-panel`)
5. Add Environment Variables:
   - `NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY`
   - `CLERK_SECRET_KEY`
   - `OWNER_KEY` (your owner key)
   - Optional: `KV_REST_API_URL`, `KV_REST_API_TOKEN` (Upstash Redis)
6. Click **Deploy**

After deploy, your site will be at `https://imgui-hook-xxx.vercel.app` or similar.

**Update Loader**: Edit `panel_url.txt` next to Loader.exe with your Vercel URL.
