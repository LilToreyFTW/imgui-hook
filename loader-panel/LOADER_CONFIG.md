# Loader Panel URL Config

After deploying to Vercel, update the Loader (`Loader/main.cpp`):

```cpp
const wchar_t* PANEL_HOST = L"your-actual-app.vercel.app";
```

Replace `your-actual-app.vercel.app` with your Vercel deployment URL (e.g. `red-eyed-ninja-loader.vercel.app`).

## Deploy to Vercel

1. Push the `loader-panel` folder to GitHub
2. Go to https://vercel.com → New Project → Import
3. Add Clerk env vars: `NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY`, `CLERK_SECRET_KEY`
4. Add sign-in/sign-up URLs: `/sign-in`, `/sign-up`
5. Deploy

## If You Delete the Vercel Deployment

The Loader requires internet connection to the panel. If you delete the Vercel deployment:

- `/api/ping` → 404
- `/api/validate` → 404
- Loader shows "Ping: No connection"
- Key validation fails
- **No one can use the Loader** – full control on your side
