import { SignIn } from "@clerk/nextjs";
import Link from "next/link";

const hasClerkKeys =
  !!process.env.NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY?.startsWith("pk_") &&
  !!process.env.CLERK_SECRET_KEY?.startsWith("sk_");

export default function SignInPage() {
  if (!hasClerkKeys) {
    return (
      <div className="flex min-h-[calc(100vh-4rem)] flex-col items-center justify-center p-8">
        <h1 className="mb-4 text-2xl font-bold text-white">Sign in</h1>
        <p className="mb-6 max-w-md text-center text-zinc-400">
          Add <code className="rounded bg-zinc-800 px-2 py-1">NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY</code> (pk_test_...) and{" "}
          <code className="rounded bg-zinc-800 px-2 py-1">CLERK_SECRET_KEY</code> (sk_test_...) in Vercel → Settings → Environment Variables, then redeploy.
        </p>
        <Link href="/" className="rounded bg-[#cd5c5c] px-6 py-2 text-white hover:bg-[#b84a4a]">
          Back to Home
        </Link>
      </div>
    );
  }
  return (
    <div className="flex min-h-[calc(100vh-4rem)] flex-col items-center justify-center p-8">
      <h1 className="mb-2 text-2xl font-bold text-white">Sign in to RedEyedNinja-X</h1>
      <p className="mb-6 text-center text-sm text-zinc-400">
        After signing in, go to Dashboard to get your Loader key.
      </p>
      <SignIn
        appearance={{
          elements: {
            rootBox: "mx-auto",
          },
        }}
      />
    </div>
  );
}
