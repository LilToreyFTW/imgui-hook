"use client";

import Link from "next/link";
import { usePathname } from "next/navigation";
import { SignInButton, SignUpButton, SignedIn, SignedOut, UserButton } from "@clerk/nextjs";
import { useAuthConfig } from "@/lib/auth-config";

export default function Nav() {
  const path = usePathname();
  const hasClerk = useAuthConfig();

  const link = (href: string, label: string) => (
    <Link
      href={href}
      className={`px-4 py-2 rounded text-sm font-medium transition ${
        path === href || (href !== "/" && path.startsWith(href))
          ? "bg-[#cd5c5c] text-white"
          : "text-zinc-400 hover:text-white hover:bg-zinc-800"
      }`}
    >
      {label}
    </Link>
  );

  return (
    <header className="border-b border-zinc-800 bg-[#0a0a0a]">
      <nav className="mx-auto flex h-14 max-w-5xl items-center justify-between px-6">
        <div className="flex items-center gap-1">
          <Link href="/" className="mr-6 font-bold text-white">
            RedEyedNinja-X
          </Link>
          {link("/", "Home")}
          <a
            href="/RedEyedNinja-X-Loader.exe"
            download="RedEyedNinja-X-Loader.exe"
            className="px-4 py-2 rounded text-sm font-medium text-zinc-400 hover:text-white hover:bg-zinc-800 transition"
          >
            Download
          </a>
          {link("/purchase", "Purchase")}
          {link("/dashboard", "Dashboard")}
          {link("/admin", "Admin")}
        </div>
        <div className="flex items-center gap-2">
          {hasClerk ? (
            <>
              <SignedOut>
                <SignInButton forceRedirectUrl="/dashboard">
                  <button className="rounded bg-zinc-800 px-4 py-2 text-sm font-medium text-zinc-300 hover:bg-zinc-700">
                    Sign In
                  </button>
                </SignInButton>
                <SignUpButton forceRedirectUrl="/dashboard">
                  <button className="rounded bg-[#cd5c5c] px-4 py-2 text-sm font-medium text-white hover:bg-[#b84a4a]">
                    Sign Up
                  </button>
                </SignUpButton>
              </SignedOut>
              <SignedIn>
                <UserButton />
              </SignedIn>
            </>
          ) : (
            <>
              <Link href="/sign-in" className="rounded bg-zinc-800 px-4 py-2 text-sm font-medium text-zinc-300 hover:bg-zinc-700">
                Sign In
              </Link>
              <Link href="/sign-up" className="rounded bg-[#cd5c5c] px-4 py-2 text-sm font-medium text-white hover:bg-[#b84a4a]">
                Sign Up
              </Link>
            </>
          )}
        </div>
      </nav>
    </header>
  );
}
