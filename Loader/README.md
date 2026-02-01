# Loader

Injects the menu DLL (`cheese.dll`) into the FiveM game process.

## Build

**Visual Studio:** Open `Loader.sln`, build Release|x64.

**Command line:**
```bat
cd Loader
msbuild Loader.sln /p:Configuration=Release /p:Platform=x64
```

Or run `build.bat`.

Output: `Loader.exe` in the parent `cheese` folder.

## Usage

1. Build the menu DLL as `cheese.dll` and place it in the same folder as `Loader.exe`
2. Start FiveM and join a server (wait for the game to fully load)
3. Run `Loader.exe`
4. Press INSERT in-game to open the menu

## Requirements

- `cheese.dll` in the same folder as Loader.exe
- FiveM running (any build: 2060 through 3717)
- Run as Administrator if injection fails
