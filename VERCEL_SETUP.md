# Vercel + Loader Panel Setup – Full Registration & Login

Follow these steps so the panel and Loader work for everyone.

---

## 1. Add Environment Variables in Vercel

1. Go to [vercel.com](https://vercel.com) → your project → **Settings** → **Environment Variables**
2. Add each variable below for **Production**, **Preview**, and **Development**:

| Variable | Value | Where to get it |
|----------|-------|-----------------|
| `NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY` | `pk_test_...` | [Clerk Dashboard](https://dashboard.clerk.com) → API Keys → **Publishable** |
| `CLERK_SECRET_KEY` | `sk_test_...` | Clerk Dashboard → API Keys → **Secret** |
| `OWNER_KEY` | `REN-OWNER-7F3E9A2B4C8D1E60` | (Use this exact value for your owner key) |
| `ADMIN_USER_IDS` | `user_37mP4npI7kykjmfLG1UzKcV6q9m` | Your Clerk user ID (Dashboard → Users → your user) |

**Clerk keys**  
- Publishable key starts with `pk_test_` or `pk_live_`  
- Secret key starts with `sk_test_` or `sk_live_`  

**Your owner key**  
- `REN-OWNER-7F3E9A2B4C8D1E60` – use this in the Loader to sign in. It bypasses HWID.

3. Click **Save** for each variable.
4. Go to **Deployments** → **⋯** on latest deployment → **Redeploy** (required for new env vars to take effect).

---

## 2. Configure the Loader (Your PC)

1. Put the Loader and `cheese.dll` in the same folder.
2. Create `panel_url.txt` in that folder with your Vercel URL on a single line:
   ```
   https://imgui-hook.vercel.app
   ```
   Replace `imgui-hook.vercel.app` with your actual Vercel domain if different.
3. Open the Loader.
4. Enter your owner key: `REN-OWNER-7F3E9A2B4C8D1E60`
5. Click **Login** – you should see the Inject screen.

---

## 3. Registration Flow (New Users)

1. **Sign up** – Go to your site → **Sign Up** (Clerk).
2. **Purchase request** – Go to **Purchase** → pick a plan → enter email and panel username → **Submit**.
3. **Admin approval** – You sign in → go to **Admin** → verify payment → **Approve & Create Key**.
4. **User gets key** – User signs in → goes to **Dashboard** → sees their key.
5. **Loader login** – User enters the key in the Loader → Login (first use binds to their computer).

---

## 4. Your Admin Login Flow

1. Sign up or sign in on the panel with your Clerk account.
2. Go to **Dashboard** – you’ll see your owner key (you’re in `ADMIN_USER_IDS`).
3. Copy the key or use `REN-OWNER-7F3E9A2B4C8D1E60` in the Loader.

---

## 5. Troubleshooting

| Issue | Fix |
|-------|-----|
| Loader: "Invalid key" | Confirm `OWNER_KEY` is set in Vercel and matches exactly. Confirm `panel_url.txt` points to your Vercel URL. |
| Sign Up / Sign In don’t work | Add both `NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY` and `CLERK_SECRET_KEY` in Vercel, then redeploy. |
| Dashboard shows no key | For admins: check `ADMIN_USER_IDS` contains your Clerk user ID. For customers: approve their purchase in Admin. |
| "Add Clerk keys" message | Clerk env vars are missing or invalid. Add them and redeploy. |

---

## 6. Optional: Redis for Customer Keys (Vercel)

For customer keys to persist on Vercel, add:

- `UPSTASH_REDIS_REST_URL`
- `UPSTASH_REDIS_REST_TOKEN`

From [Upstash](https://upstash.com). Without Redis, only the owner key (from `OWNER_KEY`) works; customer keys won’t be stored reliably.
