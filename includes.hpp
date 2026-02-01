#pragma once

#include <Windows.h>
#include "CustomWinApi.hpp"
#include <d3d11.h>
#include <dxgi.h>
#include <cstdint>
#include <cstddef>
#include <xmmintrin.h>
#include <cmath>
#include <thread>
#include <chrono>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#define DEBUG 0

#define SAFE_CALL(f) f
#define E(x) (x)

namespace Memory {
    inline uintptr_t PatternScan(const char* sig, void* param1, void* param2) {
        (void)param1;
        uintptr_t moduleBase = (uintptr_t)GetModuleHandleA(NULL);
        if (!moduleBase) return 0;
        uintptr_t found = CustomAPII::ScanSignature(moduleBase, sig, 0);
        if (!found) return 0;
        int rva = param2 ? (int)(intptr_t)param2 : 0;
        if (rva >= 4 && rva <= 16) {
            int32_t rel = *(int32_t*)(found + rva - 4);
            return found + rva + rel;
        }
        return found;
    }
}

#define LI_FN(name) \
    [&]() { return name; }()

// LI_FN(SetWindowLongPtrA).safe_cached()() replacement
template<typename F>
struct SafeCached {
    F fn;
    auto operator()() { return fn(); }
};
template<typename F> SafeCached<F> MakeSafeCached(F f) { return {f}; }

struct PVector3 { float x, y, z; PVector3() : x(0),y(0),z(0){} PVector3(float a,float b,float c):x(a),y(b),z(c){} };
