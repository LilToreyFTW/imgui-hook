'use client';

import { useAuth } from '@clerk/nextjs';
import { useRouter } from 'next/navigation';
import { useEffect, useState } from 'react';
import { useAuthConfig } from '@/lib/auth-config';

function DashboardContent() {
  const { isSignedIn, isLoaded } = useAuth();
  const router = useRouter();
  const [key, setKey] = useState<string | null>(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    if (!isLoaded) return;
    if (!isSignedIn) {
      router.push('/');
      return;
    }
    fetch('/api/key')
      .then((r) => r.json())
      .then((d) => {
        setKey(d.key ?? null);
        setLoading(false);
      })
      .catch(() => setLoading(false));
  }, [isLoaded, isSignedIn, router]);

  const copyKey = () => {
    if (key) navigator.clipboard.writeText(key);
  };

  if (!isLoaded || !isSignedIn) return null;

  return (
    <div className="mx-auto max-w-2xl p-8">
      <h1 className="text-2xl font-bold">Dashboard</h1>
      <p className="mt-2 text-zinc-600 dark:text-zinc-400">
        Your subscription key for the RedEyedNinja-X Loader.
      </p>

      <div className="mt-8 rounded-lg border border-zinc-200 dark:border-zinc-800 p-6">
        {loading ? (
          <p>Loading...</p>
        ) : key ? (
          <>
            <p className="text-sm font-medium text-zinc-500">Subscription Key</p>
            <code className="mt-2 block break-all rounded bg-zinc-100 dark:bg-zinc-900 px-3 py-2 font-mono text-sm">
              {key}
            </code>
            <button
              onClick={copyKey}
              className="mt-4 rounded-lg bg-zinc-200 dark:bg-zinc-800 px-4 py-2 text-sm font-medium hover:bg-zinc-300 dark:hover:bg-zinc-700"
            >
              Copy Key
            </button>
          </>
        ) : (
          <>
            <p className="text-zinc-600 dark:text-zinc-400">
              You don&apos;t have a key yet. Purchase a plan and submit the form so we can configure your key.
            </p>
            <a
              href="/purchase"
              className="mt-4 inline-block rounded-lg bg-[#cd5c5c] px-4 py-2 text-white font-medium hover:bg-[#b84a4a]"
            >
              Go to Purchase
            </a>
          </>
        )}
      </div>

      <p className="mt-6 text-sm text-zinc-500">
        Enter this key in the Loader and click Login. The key binds to your computer on first use (HWID).
      </p>
    </div>
  );
}

export default function DashboardPage() {
  const hasClerk = useAuthConfig();
  if (!hasClerk) {
    return (
      <div className="p-8">
        <h1 className="text-2xl font-bold">Dashboard</h1>
        <p className="mt-2 text-zinc-400">Add Clerk keys in Vercel to enable dashboard.</p>
      </div>
    );
  }
  return <DashboardContent />;
}
