import { NextRequest, NextResponse } from 'next/server';
import { addPurchaseRequest } from '@/lib/keys';

export async function POST(req: NextRequest) {
  try {
    const body = await req.json();
    const email = body?.email?.trim();
    const username = body?.username?.trim();
    const plan = body?.plan?.trim();

    if (!email || !username || !plan) {
      return NextResponse.json({ error: 'Email, username, and plan required' }, { status: 400 });
    }

    await addPurchaseRequest(email, username, plan);
    return NextResponse.json({ ok: true, message: 'Request received. Your key will be configured after payment verification.' });
  } catch (e) {
    return NextResponse.json({ error: 'Failed to submit' }, { status: 500 });
  }
}
