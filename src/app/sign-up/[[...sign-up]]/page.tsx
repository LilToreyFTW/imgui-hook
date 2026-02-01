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
          Add Clerk keys in Vercel Environment Variables to enable sign up.
        </p>
        <Link href="/" className="rounded bg-[#cd5c5c] px-6 py-2 text-white hover:bg-[#b84a4a]">
          Back to Home
        </Link>
      </div>
    );
  }
  return (
    <div className="flex min-h-[calc(100vh-4rem)] flex-col items-center justify-center p-8">
      <h1 className="mb-8 text-2xl font-bold">Create account</h1>
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
