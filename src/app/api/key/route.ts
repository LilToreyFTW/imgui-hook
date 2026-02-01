import { NextResponse } from 'next/server';
import { auth, currentUser } from '@clerk/nextjs/server';
import { getKeyByEmail, getSubscriptionForUser, getUserKey } from '@/lib/keys';

const ADMIN_USER_IDS = (process.env.ADMIN_USER_IDS ?? '').split(',').map((s) => s.trim()).filter(Boolean);

export async function GET() {
  const { userId } = await auth();
  if (!userId) return NextResponse.json({ error: 'Unauthorized' }, { status: 401 });

  const user = await currentUser();
  const email = user?.emailAddresses?.[0]?.emailAddress;
  const subscription = await getSubscriptionForUser(userId, email);

  if (!subscription) {
    const ownerKey = process.env.OWNER_KEY?.trim();
    const ownerEmail = process.env.OWNER_EMAIL?.trim().toLowerCase();
    const isOwner = ownerKey && (
      ADMIN_USER_IDS.includes(userId) ||
      (email && ownerEmail && email.toLowerCase() === ownerEmail)
    );
    if (isOwner) {
      return NextResponse.json({ key: ownerKey, plan: 'Owner', isOwner: true });
    }
    return NextResponse.json({ key: null });
  }

  return NextResponse.json({
    key: subscription.key,
    plan: subscription.plan,
    expiresAt: subscription.expiresAt,
    isOwner: subscription.isOwner,
  });
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
