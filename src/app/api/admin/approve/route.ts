import { NextRequest, NextResponse } from 'next/server';
import { auth } from '@clerk/nextjs/server';
import { createKeyForEmail, loadPurchases, savePurchases } from '@/lib/keys';

const ADMIN_USER_IDS = (process.env.ADMIN_USER_IDS ?? '').split(',').map((s) => s.trim()).filter(Boolean);

export async function POST(req: NextRequest) {
  const { userId } = await auth();
  if (!userId) return NextResponse.json({ error: 'Unauthorized' }, { status: 401 });
  if (ADMIN_USER_IDS.length && !ADMIN_USER_IDS.includes(userId)) {
    return NextResponse.json({ error: 'Forbidden' }, { status: 403 });
  }
  try {
    const { id, email } = await req.json();
    if (!id || !email) return NextResponse.json({ error: 'id and email required' }, { status: 400 });

    const key = await createKeyForEmail(email);

    const purchases = await loadPurchases();
    const idx = purchases.findIndex((p) => p.id === id);
    if (idx >= 0) {
      purchases[idx].status = 'approved';
      await savePurchases(purchases);
    }

    return NextResponse.json({ ok: true, key });
  } catch {
    return NextResponse.json({ error: 'Failed' }, { status: 500 });
  }
}
