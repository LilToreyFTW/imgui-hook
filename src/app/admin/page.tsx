'use client';

import { useAuth } from '@clerk/nextjs';
import { useEffect, useState } from 'react';
import Link from 'next/link';

interface PurchaseRequest {
  id: string;
  email: string;
  username: string;
  plan: string;
  createdAt: string;
  status: string;
}

export default function AdminPage() {
  const { userId, isLoaded } = useAuth();
  const [purchases, setPurchases] = useState<PurchaseRequest[]>([]);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    fetch('/api/admin/purchases')
      .then((r) => r.json())
      .then((d) => {
        setPurchases(d.purchases ?? []);
        setLoading(false);
      })
      .catch(() => setLoading(false));
  }, []);

  const approve = async (id: string, email: string) => {
    const res = await fetch('/api/admin/approve', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ id, email }),
    });
    if (res.ok) {
      setPurchases((p) => p.map((x) => (x.id === id ? { ...x, status: 'approved' } : x)));
    }
  };

  if (!isLoaded || !userId) return <div className="p-8">Loading...</div>;

  return (
    <div className="space-y-8">
      <div className="flex items-center justify-between">
        <h1 className="text-2xl font-bold text-white">Admin – Purchase Requests</h1>
        <Link href="/dashboard" className="text-sm text-zinc-400 hover:text-white">Back to Dashboard</Link>
      </div>
      <p className="text-zinc-400">Verify Cash App payment, then click Approve to create a key for the user.</p>
      {loading ? (
        <p className="text-zinc-500">Loading...</p>
      ) : purchases.length === 0 ? (
        <p className="text-zinc-500">No purchase requests yet.</p>
      ) : (
        <div className="space-y-4">
          {purchases.map((p) => (
            <div key={p.id} className="rounded-lg border border-zinc-800 bg-[#121212] p-6">
              <div className="flex flex-wrap items-center justify-between gap-4">
                <div>
                  <p className="font-medium text-white">{p.email}</p>
                  <p className="text-sm text-zinc-500">Username: {p.username} · Plan: {p.plan}</p>
                  <p className="text-xs text-zinc-600">{new Date(p.createdAt).toLocaleString()}</p>
                </div>
                <div>
                  {p.status === 'approved' ? (
                    <span className="text-green-400">Approved</span>
                  ) : (
                    <button
                      onClick={() => approve(p.id, p.email)}
                      className="rounded-lg bg-[#cd5c5c] px-4 py-2 text-sm font-medium text-white hover:bg-[#b84a4a]"
                    >
                      Approve & Create Key
                    </button>
                  )}
                </div>
              </div>
            </div>
          ))}
        </div>
      )}
    </div>
  );
}
