# Backdoor Audit: menu_base (RedEngine)

**Audit Date:** January 2025  
**Integration:** RedEyedNinja-X - CLEAN implementation, NO code from menu_base copied

## Removed / Not Integrated (Security)

### 1. Discord Process Killing (core.cpp:44-61)
- **DANGER:** Renames Discord RPC module, repeatedly kills Discord.exe
- **Harms:** User's computer, breaks Discord
- **Status:** NOT included

### 2. Remote Code Download - ExecuteMenu (functions.cpp:275)
- **DANGER:** `URLDownloadToFileA` from `https://redengine.eu/download.php?menu=`
- **Harms:** Downloads and executes arbitrary code from remote server
- **Status:** NOT included

### 3. MountInit - Lua Scheduler Hijack (functions.cpp:416-421)
- **DANGER:** Downloads from `https://redengine.eu/fivem/lua/` to path spoofing Steam
- **Harms:** Replaces FiveM core files, could inject malware
- **Status:** NOT included

### 4. Auth - redengine.eu (Auth.h:177)
- **Risk:** Connects to auth.redengine.eu, sends user SID
- **Replaced with:** RedEyedNinjaAuth → localhost:6969 (your zAuthServer)

### 5. "Tampering Detected" scare (core.cpp:37)
- Fake threat message
- **Status:** NOT included

## What Was Built (Clean)

- **RedEyedNinjaAuth:** Connects to ws/tcp localhost:6969 for your zAuthServer
- **ServerResources:** Reads from %localappdata%\FiveM\...\nui-storage\ (local cache only)
- **Menu:** Full RedEyedNinja-X UI, resource browser, executor placeholder
- **No external downloads, no process killing, no redengine.eu**
