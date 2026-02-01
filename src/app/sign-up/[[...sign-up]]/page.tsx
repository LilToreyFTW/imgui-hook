import { SignUp } from "@clerk/nextjs";
import Link from "next/link";

const hasClerkKeys =
  !!process.env.NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY?.startsWith("pk_") &&
  !!process.env.CLERK_SECRET_KEY?.startsWith("sk_");

export default function SignUpPage() {
  if (!hasClerkKeys) {
    return (
      <div className="flex min-h-[calc(100vh-4rem)] flex-col items-center justify-center p-8">
        <h1 className="mb-4 text-2xl font-bold text-white">Create account</h1>
        <p className="mb-6 max-w-md text-center text-zinc-400">
          Add NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY and CLERK_SECRET_KEY in Vercel → Settings → Environment Variables, then redeploy. See VERCEL_SETUP.md for full instructions.
        </p>
        <Link href="/" className="rounded bg-[#cd5c5c] px-6 py-2 text-white hover:bg-[#b84a4a]">
          Back to Home
        </Link>
      </div>
    );
  }
  return (
    <div className="flex min-h-[calc(100vh-4rem)] flex-col items-center justify-center p-8">
      <h1 className="mb-2 text-2xl font-bold text-white">Create account</h1>
      <p className="mb-6 text-center text-sm text-zinc-400">
        After signing up, go to Purchase to get a key, then Dashboard.
      </p>
      <SignUp
        appearance={{
          elements: {
            rootBox: "mx-auto",
          },
        }}
      />
    </div>
  );
}
