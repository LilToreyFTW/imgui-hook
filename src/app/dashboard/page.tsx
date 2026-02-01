'use client';

import { useAuth, useUser } from '@clerk/nextjs';
import { useRouter } from 'next/navigation';
import { useEffect, useState } from 'react';
import { useAuthConfig } from '@/lib/auth-config';

interface Subscription {
  key: string;
  plan?: string;
  expiresAt?: string;
  isOwner?: boolean;
}

function DashboardContent() {
  const { isSignedIn, isLoaded, getToken } = useAuth();
  const { user } = useUser();
  const router = useRouter();
  const [subscription, setSubscription] = useState<Subscription | null>(null);
  const [loading, setLoading] = useState(true);
  const [activeTab, setActiveTab] = useState<'key' | 'subscription'>('key');

  useEffect(() => {
    if (!isLoaded) return;
    if (!isSignedIn) {
      router.push('/');
      return;
    }
    (async () => {
      try {
        const token = await getToken();
        const res = await fetch('/api/key', {
          headers: token ? { Authorization: `Bearer ${token}` } : {},
        });
        const d = await res.json();
        if (d.key) {
          setSubscription({
            key: d.key,
            plan: d.plan ?? 'Lifetime',
            expiresAt: d.expiresAt,
            isOwner: d.isOwner,
          });
        } else {
          setSubscription(null);
        }
      } catch {
        setSubscription(null);
      } finally {
        setLoading(false);
      }
    })();
  }, [isLoaded, isSignedIn, getToken, router]);

  const copyKey = () => {
    if (subscription?.key) navigator.clipboard.writeText(subscription.key);
  };

  if (!isLoaded || !isSignedIn) return null;

  return (
    <div className="mx-auto max-w-2xl p-8">
      <div className="flex items-center gap-4">
        <h1 className="text-2xl font-bold">Dashboard</h1>
        {subscription?.isOwner && (
          <span className="rounded bg-[#cd5c5c] px-2 py-1 text-xs font-bold text-white">
            OWNER
          </span>
        )}
      </div>
      <p className="mt-2 text-zinc-600 dark:text-zinc-400">
        Your subscription key for the RedEyedNinja-X Loader.
      </p>

      {/* Tabs */}
      <div className="mt-6 flex gap-2 border-b border-zinc-800">
        <button
          type="button"
          onClick={() => setActiveTab('key')}
          className={`px-4 py-2 text-sm font-medium transition ${activeTab === 'key' ? 'border-b-2 border-[#cd5c5c] text-white' : 'text-zinc-400 hover:text-white'}`}
        >
          Key & Download
        </button>
        <button
          type="button"
          onClick={() => setActiveTab('subscription')}
          className={`px-4 py-2 text-sm font-medium transition ${activeTab === 'subscription' ? 'border-b-2 border-[#cd5c5c] text-white' : 'text-zinc-400 hover:text-white'}`}
        >
          My Subscription
        </button>
      </div>

      <div className="mt-8 rounded-lg border border-zinc-200 dark:border-zinc-800 p-6">
        {loading ? (
          <p>Loading...</p>
        ) : subscription ? (
          <>
            {activeTab === 'key' && (
              <>
                <p className="text-sm font-medium text-zinc-500">Subscription Key</p>
                <code className="mt-2 block break-all rounded bg-zinc-100 dark:bg-zinc-900 px-3 py-2 font-mono text-sm">
                  {subscription.key}
                </code>
            <button
              onClick={copyKey}
              className="mt-4 rounded-lg bg-zinc-200 dark:bg-zinc-800 px-4 py-2 text-sm font-medium hover:bg-zinc-300 dark:hover:bg-zinc-700"
            >
              Copy Key
            </button>
                <div className="mt-6 flex flex-wrap gap-4">
                  <a
                    href="/api/download/loader"
                    download="RedEyedNinja-X-Loader.exe"
                    className="rounded-lg bg-[#cd5c5c] px-4 py-2 text-sm font-medium text-white hover:bg-[#b84a4a]"
                  >
                    Download Loader
                  </a>
                  <a
                    href="/api/download/cheese"
                    download="cheese.dll"
                    className="rounded-lg bg-[#cd5c5c] px-4 py-2 text-sm font-medium text-white hover:bg-[#b84a4a]"
                  >
                    Download cheese.dll
                  </a>
                </div>
              </>
            )}
            {activeTab === 'subscription' && (
              <div className="space-y-4">
                <div>
                  <p className="text-sm font-medium text-zinc-500">Plan</p>
                  <p className="mt-1 text-lg font-semibold text-white">{subscription.plan}</p>
                </div>
                {subscription.expiresAt && (
                  <div>
                    <p className="text-sm font-medium text-zinc-500">Expires</p>
                    <p className="mt-1 text-white">
                      {new Date(subscription.expiresAt).toLocaleDateString()}
                    </p>
                  </div>
                )}
                {subscription.isOwner && (
                  <div>
                    <p className="text-sm font-medium text-zinc-500">Status</p>
                    <p className="mt-1 text-[#cd5c5c] font-medium">Owner – Full Access</p>
                  </div>
                )}
                {user && (
                  <div className="border-t border-zinc-700 pt-4 mt-4">
                    <p className="text-sm font-medium text-zinc-500">Profile</p>
                    <p className="mt-1 text-white">{user.fullName ?? user.username ?? 'User'}</p>
                    <p className="text-sm text-zinc-400">{user.primaryEmailAddress?.emailAddress}</p>
                  </div>
                )}
              </div>
            )}
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
      <p className="mt-2 text-sm text-zinc-600 dark:text-zinc-600">
        If login fails, check that panel_url.txt next to the Loader contains your Vercel URL (e.g. https://imgui-hook.vercel.app).
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
