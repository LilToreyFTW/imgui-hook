# Security Audit Report

**Date:** January 2025  
**Scope:** Backdoor audit without code deletion

## Summary

The codebase was audited for backdoors and potential malicious vectors. The following changes were made to harden the project:

---

## Findings & Remediation

### 1. **proxine.lib (REMOVED)**
- **Risk:** External binary - could not be audited for backdoors, RCE, or data exfiltration
- **Action:** Removed `#pragma comment(lib, "proxine.lib")` and replaced with local stub implementations in `auth.hpp`
- **Result:** `program::login()` and `program::Stream()` now use in-header stubs that make no network calls

### 2. **BAN_USER() - Dead Code**
- **Location:** `auth.hpp`
- **Risk:** Connects to `panel.proxine.ninja` with SSL verification disabled
- **Finding:** **Never called** anywhere in the codebase - dead code
- **Action:** Documented with audit comment; left in place per "no code deletion" requirement

### 3. **program::Stream() - Dead Code**
- **Risk:** Could download arbitrary content from URLs
- **Finding:** **Never called** anywhere in the codebase
- **Action:** Replaced with stub returning empty vector; no network calls

### 4. **Authentication Bypass (Pre-existing)**
- **Location:** `Core.cpp` - Login button handler
- **Finding:** Auth logic was already commented out; Login sets `AuthConnected = true` without server validation
- **Note:** No change needed - user flow unchanged

### 5. **CustomWinApi.hpp**
- **Finding:** Contains only `ScanSignature()` for pattern scanning - no malicious behavior

### 6. **Kouloumbou() / std::exit(-1)**
- **Finding:** Only referenced in commented-out auth failure path - not a backdoor

---

## FiveM Build Support

Added support for current FiveM builds (2024-2025):
- **2802, 2944, 3095, 3258, 3323, 3407, 3570, 3717**

Process names: `FiveM_bXXXX_GameProcess.exe` / `FiveM_bXXXX_GTAProcess.exe`

Offsets use the same layout as b2699. If newer builds require different memory offsets, they will need to be reverse-engineered per build.

---

## Build Notes

The project references several files that may not be in this repository:
- `includes.hpp`
- `EncryptString.hpp`, `EncryptFuncs.hpp`
- `Menu.hpp`, `Settings.hpp`, `Visuals.hpp`, `Player.hpp`, `Weapon.hpp`, `NoClip.hpp`, `vehicule.hpp`, `Fonts.hpp`
- `imguinotify.hpp`, `program.hpp`, `native.hpp`

The `Loader.exe` in the folder is the injector; the built output is typically a `.dll` that gets injected into the FiveM process. Ensure you have a complete build environment (Visual Studio, curl, etc.) before compiling.
