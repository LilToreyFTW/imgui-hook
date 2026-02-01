# RedEyedNinja-X Loader Setup

## Loader Flow

1. **Open Loader** – Dark UI with ping at top
2. **Enter subscription key** – From the loader panel dashboard
3. **Click Login** – Validates key + HWID against panel API
   - First use: Key binds to this computer's HWID
   - Later: Key must match this computer (prevents sharing)
4. **Inject screen** – Key verified, click Inject
5. **FiveM** – Running → Injects immediately. Not running → Auto-injects when detected
6. **In-game** – Press INSERT or F5 for mod menu

## Owner Key (You)

Your owner key: **REN-OWNER-7F3E9A2B4C8D1E60**

- Bypasses HWID lock (works on any PC)
- Set `OWNER_KEY=REN-OWNER-7F3E9A2B4C8D1E60` in Vercel env vars
- Enter in Loader and click Login

See `OWNER_KEY.txt` for details.

## Auto-Login (credentials.json)

On successful login, the loader saves your key to `credentials.json` in the loader folder. Next launch, it auto-logs in. All loader exes (Loader.exe, Runtime_*.exe) share this file.

## Random Exe Name

Each launch copies itself to `Runtime_XXXX.exe` and runs that. The new exe shows the UI. Keeps the executable name changing.

## Panel URL Config

Place `panel_url.txt` next to Loader.exe (same folder as cheese.dll):

```
https://your-loader-panel.vercel.app
```

Or for local dev:
```
http://localhost:3000
```

Default (if no file): `red-eyed-ninja-loader.vercel.app`

## Deploy to Vercel

1. `cd loader-panel`
2. Copy `.env.local.example` to `.env.local`
3. Add Clerk keys from https://dashboard.clerk.com
4. Add `OWNER_KEY=REN-OWNER-7F3E9A2B4C8D1E60`
5. (Optional) Add Upstash Redis from Vercel Marketplace for key storage
6. `npx vercel` or connect repo to Vercel for auto-deploy
7. Add env vars in Vercel dashboard
8. Update `panel_url.txt` with your deployment URL (e.g. `https://loader-panel-xxx.vercel.app`)

## Ping Connection

Loader pings `/api/ping` every 5 seconds. If Vercel page is deleted, ping fails and loader shows "No connection."
