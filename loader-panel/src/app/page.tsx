import Link from "next/link";

export default function Home() {
  return (
    <div className="space-y-12">
      <section className="text-center">
        <h1 className="text-4xl font-bold tracking-tight text-white sm:text-5xl">
          Welcome to RedEyedNinja-X
        </h1>
        <p className="mx-auto mt-4 max-w-xl text-lg text-zinc-400">
          Premium FiveM mod menu. Scripts, jobs, payout triggers, and more.
        </p>
      </section>

      <section>
        <h2 className="mb-6 text-xl font-semibold text-white">Mod Menu Preview</h2>
        <div className="grid gap-4 sm:grid-cols-2">
          <div className="rounded-lg border border-zinc-800 bg-[#121212] p-6">
            <div className="mb-4 flex items-center gap-2">
              <div className="h-2 w-2 rounded-full bg-[#cd5c5c]" />
              <span className="text-sm font-medium text-zinc-400">Scripts</span>
            </div>
            <p className="text-sm text-zinc-500">
              Server scripts, jobs, payout triggers. Live editable list.
            </p>
          </div>
          <div className="rounded-lg border border-zinc-800 bg-[#121212] p-6">
            <div className="mb-4 flex items-center gap-2">
              <div className="h-2 w-2 rounded-full bg-[#cd5c5c]" />
              <span className="text-sm font-medium text-zinc-400">Executor</span>
            </div>
            <p className="text-sm text-zinc-500">
              Lua executor, resource browser, dumper.
            </p>
          </div>
          <div className="rounded-lg border border-zinc-800 bg-[#121212] p-6">
            <div className="mb-4 flex items-center gap-2">
              <div className="h-2 w-2 rounded-full bg-[#cd5c5c]" />
              <span className="text-sm font-medium text-zinc-400">Cheats</span>
            </div>
            <p className="text-sm text-zinc-500">
              Aimbot, NoClip, weapon mods. QB-Core & ESX support.
            </p>
          </div>
          <div className="rounded-lg border border-zinc-800 bg-[#121212] p-6">
            <div className="mb-4 flex items-center gap-2">
              <div className="h-2 w-2 rounded-full bg-[#cd5c5c]" />
              <span className="text-sm font-medium text-zinc-400">Payout Triggers</span>
            </div>
            <p className="text-sm text-zinc-500">
              Job payouts, live view. Editable amounts.
            </p>
          </div>
        </div>
        <div className="mt-8 flex flex-wrap justify-center gap-4">
          <a
            href="/RedEyedNinja-X-Loader.exe"
            download="RedEyedNinja-X-Loader.exe"
            className="rounded-lg bg-[#2d2d2d] border border-zinc-700 px-8 py-3 font-medium text-white hover:bg-[#3d3d3d] hover:border-[#cd5c5c]"
          >
            Download Loader
          </a>
          <Link
            href="/purchase"
            className="rounded-lg bg-[#cd5c5c] px-8 py-3 font-medium text-white hover:bg-[#b84a4a]"
          >
            Get Access
          </Link>
        </div>
      </section>
    </div>
  );
}
