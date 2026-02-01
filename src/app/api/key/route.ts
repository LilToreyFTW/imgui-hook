import { NextResponse } from 'next/server';
import { auth, currentUser } from '@clerk/nextjs/server';
import { getKeyByEmail, getUserKey } from '@/lib/keys';

const ADMIN_USER_IDS = (process.env.ADMIN_USER_IDS ?? '').split(',').map((s) => s.trim()).filter(Boolean);

export async function GET() {
  const { userId } = await auth();
  if (!userId) return NextResponse.json({ error: 'Unauthorized' }, { status: 401 });

  let key = await getUserKey(userId);
  if (!key) {
    const user = await currentUser();
    const email = user?.emailAddresses?.[0]?.emailAddress;
    if (email) key = await getKeyByEmail(email);
  }
  if (!key && ADMIN_USER_IDS.includes(userId)) {
    const ownerKey = process.env.OWNER_KEY?.trim();
    if (ownerKey) key = ownerKey;
  }
  return NextResponse.json({ key: key ?? null });
}

export async function POST() {
  const { userId } = await auth();
  if (!userId) return NextResponse.json({ error: 'Unauthorized' }, { status: 401 });

  let existing = await getUserKey(userId);
  if (!existing) {
    const user = await currentUser();
    const email = user?.emailAddresses?.[0]?.emailAddress;
    if (email) existing = await getKeyByEmail(email);
  }
  return NextResponse.json({ key: existing ?? null });
}
