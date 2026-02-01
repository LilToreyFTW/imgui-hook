# Deploy to Vercel

The Next.js app is at the **repo root** – no Root Directory setting needed.

1. Go to [vercel.com](https://vercel.com) → Your Project → **Settings** → **Environment Variables**
2. Add these variables (Production, Preview, Development):

| Variable | Value |
|----------|-------|
| `NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY` | `pk_test_...` from [Clerk Dashboard](https://dashboard.clerk.com) → API Keys |
| `CLERK_SECRET_KEY` | `sk_test_...` from Clerk Dashboard (the secret key you have) |
| `OWNER_KEY` | `REN-OWNER-7F3E9A2B4C8D1E60` |
| `ADMIN_USER_IDS` | `user_37mP4npI7kykjmfLG1UzKcV6q9m` |

3. **Publishable key** (pk_test_...) – In Clerk Dashboard → API Keys, copy the **Publishable** key. This is different from the secret key.
4. **Secret key** – Use one of your `sk_test_` keys.
5. Redeploy: **Deployments** → **⋯** → **Redeploy**

## Local dev

Copy `.env.local.example` to `.env.local`. Add Clerk keys and `OWNER_KEY`, `ADMIN_USER_IDS`.

## Update Loader

After deploy, edit `panel_url.txt` next to Loader.exe with your Vercel URL (e.g. `https://imgui-hook.vercel.app`).
