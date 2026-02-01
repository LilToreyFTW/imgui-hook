# Deploy to Vercel

See **[VERCEL_SETUP.md](./VERCEL_SETUP.md)** for full registration and login setup.

## Quick env vars

| Variable | Value |
|----------|-------|
| `NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY` | `pk_test_...` from [Clerk Dashboard](https://dashboard.clerk.com) → API Keys |
| `CLERK_SECRET_KEY` | `sk_test_...` from Clerk Dashboard |
| `OWNER_KEY` | `REN-OWNER-7F3E9A2B4C8D1E60` |
| `ADMIN_USER_IDS` | `user_37mP4npI7kykjmfLG1UzKcV6q9m` |

Add in Vercel → Settings → Environment Variables, then redeploy.

## Loader panel URL

Copy `panel_url.example.txt` to `panel_url.txt` next to Loader.exe and edit with your Vercel URL (e.g. `https://imgui-hook.vercel.app`).
