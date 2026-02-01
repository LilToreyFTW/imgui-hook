import { NextRequest, NextResponse } from 'next/server';
import { validateKey } from '@/lib/keys';

export async function POST(req: NextRequest) {
  try {
    const body = await req.json();
    const key = body?.key?.trim();
    const hwid = body?.hwid?.trim();
    if (!key) return NextResponse.json({ valid: false, message: 'Key required' }, { status: 400 });
    const ownerKey = process.env.OWNER_KEY?.trim();
    if (ownerKey && key === ownerKey) {
      return NextResponse.json({ valid: true, email: 'owner' });
    }
    if (!hwid || hwid.length < 8) {
      return NextResponse.json({ valid: false, message: 'HWID required' }, { status: 400 });
    }
    const result = await validateKey(key, hwid);
    return NextResponse.json(result);
  } catch {
    return NextResponse.json({ valid: false, message: 'Server error' }, { status: 500 });
  }
}
