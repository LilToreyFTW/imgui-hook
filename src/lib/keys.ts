import { readFile, writeFile, mkdir } from 'fs/promises';
import { join } from 'path';

const KEYS_FILE = join(process.cwd(), 'data', 'subscription-keys.json');
const REDIS_KEYS_KEY = 'ren:subscription-keys';

export interface SubscriptionKey {
  key: string;
  userId: string;
  email: string;
  plan?: string;
  hwid?: string;
  createdAt: string;
  expiresAt?: string;
  active: boolean;
}

export interface PurchaseRequest {
  id: string;
  email: string;
  username: string;
  plan: string;
  createdAt: string;
  status: 'pending' | 'approved';
}

function getRedis() {
  const url = process.env.UPSTASH_REDIS_REST_URL || process.env.KV_REST_API_URL;
  const token = process.env.UPSTASH_REDIS_REST_TOKEN || process.env.KV_REST_API_TOKEN;
  if (!url || !token) return null;
  try {
    const { Redis } = require('@upstash/redis');
    return new Redis({ url, token });
  } catch {
    return null;
  }
}

async function ensureDataDir() {
  const dir = join(process.cwd(), 'data');
  try {
    await mkdir(dir, { recursive: true });
  } catch {}
}

export async function loadKeys(): Promise<Record<string, SubscriptionKey>> {
  const redis = getRedis();
  if (redis) {
    try {
      const raw = await redis.get(REDIS_KEYS_KEY);
      if (raw) {
        const parsed = typeof raw === 'string' ? JSON.parse(raw) : raw;
        return parsed && typeof parsed === 'object' ? parsed : {};
      }
    } catch {}
    return {};
  }
  await ensureDataDir();
  try {
    const raw = await readFile(KEYS_FILE, 'utf-8');
    return JSON.parse(raw);
  } catch {
    return {};
  }
}

export async function saveKeys(keys: Record<string, SubscriptionKey>) {
  const redis = getRedis();
  if (redis) {
    try {
      await redis.set(REDIS_KEYS_KEY, JSON.stringify(keys));
    } catch {}
    return;
  }
  await ensureDataDir();
  await writeFile(KEYS_FILE, JSON.stringify(keys, null, 2));
}

export async function updateKeyHwid(key: string, hwid: string) {
  const keys = await loadKeys();
  const entry = keys[key];
  if (entry) {
    entry.hwid = hwid;
    keys[key] = entry;
    await saveKeys(keys);
  }
}

export async function validateKey(key: string, hwid?: string): Promise<{ valid: boolean; email?: string; message?: string }> {
  const ownerKey = process.env.OWNER_KEY?.trim();
  if (ownerKey && key === ownerKey) {
    return { valid: true, email: 'owner' };
  }
  const keys = await loadKeys();
  const entry = keys[key];
  if (!entry || !entry.active) return { valid: false, message: 'Invalid key' };
  if (entry.expiresAt && new Date(entry.expiresAt) < new Date())
    return { valid: false, message: 'Key expired' };
  if (!hwid || hwid.length < 8) return { valid: false, message: 'HWID required' };
  if (!entry.hwid) {
    entry.hwid = hwid;
    keys[key] = entry;
    await saveKeys(keys);
    return { valid: true, email: entry.email };
  }
  if (entry.hwid !== hwid)
    return { valid: false, message: 'Key is bound to another computer' };
  return { valid: true, email: entry.email };
}

export async function createKey(userId: string, email: string): Promise<string> {
  const keys = await loadKeys();
  const key = 'REN-' + [...crypto.getRandomValues(new Uint8Array(16))].map(b => b.toString(16).padStart(2, '0')).join('').toUpperCase();
  keys[key] = {
    key,
    userId,
    email,
    createdAt: new Date().toISOString(),
    active: true,
  };
  await saveKeys(keys);
  return key;
}

export async function getUserKey(userId: string): Promise<string | null> {
  const keys = await loadKeys();
  for (const k of Object.values(keys)) {
    if (k.userId === userId && k.active) return k.key;
  }
  return null;
}

export async function getKeyByEmail(email: string): Promise<string | null> {
  const keys = await loadKeys();
  for (const k of Object.values(keys)) {
    if (k.email.toLowerCase() === email.toLowerCase() && k.active) return k.key;
  }
  return null;
}

export async function getKeyForUser(userId: string, email?: string): Promise<string | null> {
  let key = await getUserKey(userId);
  if (!key && email) key = await getKeyByEmail(email);
  if (!key) {
    const adminIds = (process.env.ADMIN_USER_IDS ?? '').split(',').map((s) => s.trim()).filter(Boolean);
    const ownerEmail = process.env.OWNER_EMAIL?.trim().toLowerCase();
    const isOwner = adminIds.includes(userId) || (email && ownerEmail && email.toLowerCase() === ownerEmail);
    if (isOwner) key = process.env.OWNER_KEY?.trim() ?? null;
  }
  return key;
}

export interface UserSubscription {
  key: string;
  plan: string;
  expiresAt?: string;
  isOwner: boolean;
  email?: string;
}

export async function getSubscriptionForUser(userId: string, email?: string): Promise<UserSubscription | null> {
  const adminIds = (process.env.ADMIN_USER_IDS ?? '').split(',').map((s) => s.trim()).filter(Boolean);
  const ownerEmail = process.env.OWNER_EMAIL?.trim().toLowerCase();
  const ownerKey = process.env.OWNER_KEY?.trim();
  if (ownerKey && (adminIds.includes(userId) || (email && ownerEmail && email.toLowerCase() === ownerEmail))) {
    return { key: ownerKey, plan: 'Owner', isOwner: true };
  }
  const keys = await loadKeys();
  for (const entry of Object.values(keys)) {
    if (!entry.active) continue;
    if (entry.userId === userId || (email && entry.email.toLowerCase() === email.toLowerCase())) {
      return {
        key: entry.key,
        plan: entry.plan ?? 'Lifetime',
        expiresAt: entry.expiresAt,
        isOwner: false,
        email: entry.email,
      };
    }
  }
  return null;
}

export async function createKeyForEmail(email: string, plan?: string): Promise<string> {
  const keys = await loadKeys();
  const key = 'REN-' + [...crypto.getRandomValues(new Uint8Array(16))].map(b => b.toString(16).padStart(2, '0')).join('').toUpperCase();
  const expiresAt = plan ? getExpiryForPlan(plan) : undefined;
  keys[key] = {
    key,
    userId: '',
    email: email.toLowerCase(),
    plan: plan ?? 'Lifetime',
    createdAt: new Date().toISOString(),
    expiresAt,
    active: true,
  };
  await saveKeys(keys);
  return key;
}

function getExpiryForPlan(plan: string): string | undefined {
  const now = new Date();
  if (plan.includes('1 Month')) {
    now.setMonth(now.getMonth() + 1);
    return now.toISOString();
  }
  if (plan.includes('3 Month')) {
    now.setMonth(now.getMonth() + 3);
    return now.toISOString();
  }
  if (plan.includes('6 Month')) {
    now.setMonth(now.getMonth() + 6);
    return now.toISOString();
  }
  if (plan.includes('12 Month')) {
    now.setFullYear(now.getFullYear() + 1);
    return now.toISOString();
  }
  return undefined; // Lifetime
}

const PURCHASES_FILE = join(process.cwd(), 'data', 'purchase-requests.json');

export async function loadPurchases(): Promise<PurchaseRequest[]> {
  await ensureDataDir();
  try {
    const raw = await readFile(PURCHASES_FILE, 'utf-8');
    return JSON.parse(raw);
  } catch {
    return [];
  }
}

export async function savePurchases(purchases: PurchaseRequest[]) {
  await ensureDataDir();
  await writeFile(PURCHASES_FILE, JSON.stringify(purchases, null, 2));
}

export async function addPurchaseRequest(email: string, username: string, plan: string): Promise<void> {
  const purchases = await loadPurchases();
  const id = 'req-' + Date.now();
  purchases.push({
    id,
    email,
    username,
    plan,
    createdAt: new Date().toISOString(),
    status: 'pending',
  });
  await savePurchases(purchases);
}
