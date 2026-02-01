'use client';

import Image from 'next/image';
import { useState } from 'react';

const plans = [
  { name: '1 Month', price: 9.99, period: '1 month', popular: false },
  { name: '3 Months', price: 15.99, period: '3 months', popular: true },
  { name: '6 Months', price: 25.99, period: '6 months', popular: false },
  { name: '12 Months', price: 89.99, period: '12 months', popular: false },
  { name: 'Lifetime', price: 150.0, period: 'Forever', popular: false },
];

export default function PurchasePage() {
  const [selectedPlan, setSelectedPlan] = useState<string | null>(null);
  const [email, setEmail] = useState('');
  const [username, setUsername] = useState('');
  const [status, setStatus] = useState<'idle' | 'loading' | 'done' | 'error'>('idle');
  const [msg, setMsg] = useState('');

  const submit = async () => {
    if (!selectedPlan || !email.trim() || !username.trim()) {
      setMsg('Please select a plan, enter email and panel username.');
      setStatus('error');
      return;
    }
    setStatus('loading');
    setMsg('');
    try {
      const res = await fetch('/api/purchase', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ email: email.trim(), username: username.trim(), plan: selectedPlan }),
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.error || 'Failed');
      setStatus('done');
      setMsg('Submitted! We\'ll configure your key after payment verification. Check your email / dashboard.');
    } catch {
      setStatus('error');
      setMsg('Failed to submit. Try again.');
    }
  };

  return (
    <div className="space-y-12">
      <section className="text-center">
        <h1 className="text-3xl font-bold text-white sm:text-4xl">Purchase Mod Menu</h1>
        <p className="mt-2 text-zinc-400">Pay via Cash App, then submit the form to get your key.</p>
      </section>

      {/* Cash App QR */}
      <section className="flex flex-col items-center rounded-lg border border-zinc-800 bg-[#121212] p-8 sm:flex-row sm:items-start sm:justify-center sm:gap-12">
        <div className="text-center">
          <p className="mb-4 font-semibold text-white">Scan to pay with Cash App</p>
          <Image
            src="/cashapp-qr.jpg"
            alt="Cash App QR Code"
            width={220}
            height={220}
            className="rounded-lg"
          />
          <p className="mt-3 font-medium text-white">Kingsly Torey Hancock</p>
          <p className="text-[#cd5c5c]">$Toreyx2024</p>
        </div>
        <div className="mt-8 max-w-sm sm:mt-0">
          <p className="text-sm text-zinc-400">1. Scan QR and send payment for your chosen plan</p>
          <p className="mt-2 text-sm text-zinc-400">2. Fill out the form so we can configure your key</p>
        </div>
      </section>

      {/* Plans */}
      <section>
        <h2 className="mb-4 text-lg font-semibold text-white">Select plan</h2>
        <div className="grid gap-4 sm:grid-cols-2 lg:grid-cols-5">
          {plans.map((plan) => (
            <button
              key={plan.name}
              type="button"
              onClick={() => setSelectedPlan(plan.name)}
              className={`rounded-lg border p-4 text-left transition ${
                selectedPlan === plan.name
                  ? 'border-[#cd5c5c] bg-[#1a1515]'
                  : 'border-zinc-800 bg-[#121212] hover:border-zinc-700'
              }`}
            >
              <p className="font-semibold text-white">{plan.name}</p>
              <p className="text-xl font-bold text-[#cd5c5c]">${plan.price.toFixed(2)}</p>
              <p className="text-xs text-zinc-500">{plan.period}</p>
            </button>
          ))}
        </div>
      </section>

      {/* Form */}
      <section className="rounded-lg border border-zinc-800 bg-[#121212] p-8">
        <h2 className="mb-6 text-lg font-semibold text-white">Get your key</h2>
        <p className="mb-6 text-sm text-zinc-400">Enter your email and panel username so we can configure your key after payment.</p>
        <div className="space-y-4 max-w-md">
          <div>
            <label className="mb-1 block text-sm text-zinc-400">Email</label>
            <input
              type="email"
              value={email}
              onChange={(e) => setEmail(e.target.value)}
              placeholder="your@email.com"
              className="w-full rounded-lg border border-zinc-700 bg-[#0a0a0a] px-4 py-3 text-white placeholder:text-zinc-600 focus:border-[#cd5c5c] focus:outline-none"
            />
          </div>
          <div>
            <label className="mb-1 block text-sm text-zinc-400">Panel username</label>
            <input
              type="text"
              value={username}
              onChange={(e) => setUsername(e.target.value)}
              placeholder="Your sign-in email or username"
              className="w-full rounded-lg border border-zinc-700 bg-[#0a0a0a] px-4 py-3 text-white placeholder:text-zinc-600 focus:border-[#cd5c5c] focus:outline-none"
            />
          </div>
          {selectedPlan && (
            <p className="text-sm text-zinc-500">Plan: {selectedPlan}</p>
          )}
          <button
            onClick={submit}
            disabled={status === 'loading'}
            className="rounded-lg bg-[#cd5c5c] px-6 py-3 font-medium text-white hover:bg-[#b84a4a] disabled:opacity-50"
          >
            {status === 'loading' ? 'Submitting...' : 'Submit'}
          </button>
          {msg && (
            <p className={status === 'done' ? 'text-green-400' : 'text-red-400'}>{msg}</p>
          )}
        </div>
      </section>

      <section className="rounded-lg border border-zinc-800 bg-[#121212] p-8 text-center">
        <p className="text-zinc-400">Questions? Contact on Discord</p>
        <p className="mt-2 font-mono text-[#cd5c5c]">@bl0wdart</p>
      </section>
    </div>
  );
}
