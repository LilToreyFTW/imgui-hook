# RedEyedNinja-X – Final Release Setup

## Owner Recognition

- **Lil Torey** (toreyhancock10@gmail.com, bl0wdart) – Owner
- Owner badge shows in Nav and Dashboard when signed in
- ADMIN_USER_IDS = user_37mP4npI7kykjmfLG1UzKcV6q9m (Clerk user ID)
- OWNER_EMAIL = toreyhancock10@gmail.com (alternate owner check)

## Payment First – Downloads Only After Key

- **Unauthenticated users**: No access to Loader or cheese.dll downloads
- **Authenticated without key**: No downloads (must purchase first)
- **Authenticated with key**: Download Loader and cheese.dll from Dashboard only

## Status

- **Vercel (imgui-hook project)**: All env vars added
- **Loader**: Built with `imgui-hook.vercel.app` as default panel
- **panel_url.txt**: Set to `https://imgui-hook.vercel.app`
- **Protected downloads**: `/RedEyedNinja-X-Loader.exe` and `/cheese.dll` redirect to sign-in; only `/api/download/loader` and `/api/download/cheese` serve files (auth + key required)

## Required: Redeploy from Vercel Dashboard

New env vars only apply after a fresh deployment.

1. Go to [vercel.com](https://vercel.com) → **imgui-hook** project
2. Open **Deployments**
3. Click **⋯** on the latest deployment → **Redeploy**
4. Wait for the build to finish

This makes Clerk sign-up/sign-in and Loader key validation work.

---

## Environment Variables (Already Set on imgui-hook)

| Variable | Value |
|----------|-------|
| `NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY` | `pk_test_cHJlbWl1bS1hZGRlci0yNC5jbGVyay5hY2NvdW50cy5kZXYk` |
| `CLERK_SECRET_KEY` | `sk_test_ErW3I6onB9B81lNhIiVbrprOdF05qRIkpuz1KpUZAG` |
| `OWNER_KEY` | `REN-OWNER-7F3E9A2B4C8D1E60` |
| `ADMIN_USER_IDS` | `user_37mP4npI7kykjmfLG1UzKcV6q9m` |

---

## Loader Setup

1. Place `RedEyedNinja-X-Loader.exe` and `cheese.dll` in the same folder
2. Ensure `panel_url.txt` in that folder contains: `https://imgui-hook.vercel.app`
3. Run the Loader
4. Enter key: `REN-OWNER-7F3E9A2B4C8D1E60`
5. Click **Login** → Inject screen should appear

---

## Panel Flow (After Redeploy)

1. **Sign Up**: https://imgui-hook.vercel.app/sign-up
2. **Sign In**: https://imgui-hook.vercel.app/sign-in
3. **Dashboard**: https://imgui-hook.vercel.app/dashboard (shows your key when signed in)
4. **Admin**: https://imgui-hook.vercel.app/admin (approve purchases, create keys)

---

## Download (Dashboard Only)

Loader and cheese.dll are **only** downloadable from the Dashboard when signed in with an approved key. Direct URLs redirect to sign-in. Payment first, then key, then download.

## My Subscription Tab

Users with keys see a **My Subscription** tab in the Dashboard showing:
- Plan duration (1 Month, 3 Months, Lifetime, etc.)
- Expiry date (if applicable)
- Profile info (name, email)
- Owner badge for admin
