import { NextResponse } from 'next/server';
import { auth, currentUser } from '@clerk/nextjs/server';
import { getKeyForUser } from '@/lib/keys';
import { readFile } from 'fs/promises';
import { join } from 'path';

export async function GET() {
  const { userId } = await auth();
  if (!userId) return NextResponse.json({ error: 'Sign in required' }, { status: 401 });

  const user = await currentUser();
  const email = user?.emailAddresses?.[0]?.emailAddress;
  const key = await getKeyForUser(userId, email);
  if (!key) return NextResponse.json({ error: 'Payment required. Purchase a plan and get your key first.' }, { status: 403 });

  try {
    const filePath = join(process.cwd(), 'public', 'RedEyedNinja-X-Loader.exe');
    const buf = await readFile(filePath);
    return new NextResponse(buf, {
      headers: {
        'Content-Type': 'application/octet-stream',
        'Content-Disposition': 'attachment; filename="RedEyedNinja-X-Loader.exe"',
      },
    });
  } catch {
    return NextResponse.json({ error: 'File not found' }, { status: 404 });
  }
}
