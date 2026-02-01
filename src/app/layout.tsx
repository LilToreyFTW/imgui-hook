import type { Metadata } from "next";
import { ClerkProvider } from "@clerk/nextjs";
import { Geist, Geist_Mono } from "next/font/google";
import Nav from "@/components/Nav";
import Footer from "@/components/Footer";
import { AuthConfigProvider } from "@/lib/auth-config";
import "./globals.css";

const geistSans = Geist({
  variable: "--font-geist-sans",
  subsets: ["latin"],
});

const geistMono = Geist_Mono({
  variable: "--font-geist-mono",
  subsets: ["latin"],
});

export const metadata: Metadata = {
  title: "RedEyedNinja-X | FiveM Mod Menu",
  description: "RedEyedNinja-X Mod Menu & Loader",
};

const hasClerkKeys =
  !!process.env.NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY?.startsWith("pk_") &&
  !!process.env.CLERK_SECRET_KEY?.startsWith("sk_");

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  const content = (
    <AuthConfigProvider hasClerk={hasClerkKeys}>
      <html lang="en" className="dark">
        <body className={`${geistSans.variable} ${geistMono.variable} min-h-screen bg-[#0a0a0a] antialiased`}>
          <Nav />
          <main className="mx-auto max-w-5xl px-6 py-8">{children}</main>
          <Footer />
        </body>
      </html>
    </AuthConfigProvider>
  );

  if (hasClerkKeys) {
    return <ClerkProvider>{content}</ClerkProvider>;
  }
  return content;
}
