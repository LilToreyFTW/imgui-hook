# RedEyedNinja-X – Final Release Setup

## Status

- **Vercel (imgui-hook project)**: All env vars added
- **Loader**: Built with `imgui-hook.vercel.app` as default panel
- **panel_url.txt**: Set to `https://imgui-hook.vercel.app`

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

## Download

- **Loader**: https://imgui-hook.vercel.app/RedEyedNinja-X-Loader.exe
- **cheese.dll**: https://imgui-hook.vercel.app/cheese.dll
