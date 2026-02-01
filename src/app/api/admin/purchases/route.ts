import { NextResponse } from 'next/server';
import { auth } from '@clerk/nextjs/server';
import { loadPurchases } from '@/lib/keys';

const ADMIN_USER_IDS = (process.env.ADMIN_USER_IDS ?? '').split(',').map((s) => s.trim()).filter(Boolean);

export async function GET() {
  const { userId } = await auth();
  if (!userId) return NextResponse.json({ error: 'Unauthorized' }, { status: 401 });
  if (ADMIN_USER_IDS.length && !ADMIN_USER_IDS.includes(userId)) {
    return NextResponse.json({ error: 'Forbidden' }, { status: 403 });
  }
  const purchases = await loadPurchases();
  return NextResponse.json({ purchases });
}
