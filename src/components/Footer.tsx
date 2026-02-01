"use client";

export default function Footer() {
  const copyDiscord = () => {
    navigator.clipboard.writeText("@bl0wdart");
  };

  return (
    <footer className="mt-16 border-t border-zinc-800 py-8 text-center text-sm text-zinc-500">
      Contact:{" "}
      <button
        onClick={copyDiscord}
        className="font-mono text-[#cd5c5c] hover:underline"
      >
        @bl0wdart
      </button>
      {" "}on Discord
    </footer>
  );
}
