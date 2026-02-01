import { SignUp } from "@clerk/nextjs";

export default function SignUpPage() {
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
