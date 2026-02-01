import { NextResponse } from 'next/server';
import { auth, currentUser } from '@clerk/nextjs/server';
import { getKeyByEmail, getUserKey } from '@/lib/keys';

export async function GET() {
  const { userId } = await auth();
  if (!userId) return NextResponse.json({ error: 'Unauthorized' }, { status: 401 });

  let key = await getUserKey(userId);
  if (!key) {
    const user = await currentUser();
    const email = user?.emailAddresses?.[0]?.emailAddress;
    if (email) key = await getKeyByEmail(email);
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
