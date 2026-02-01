#pragma once

#include "imgui/imgui.h"
#include <string>

namespace Menu {
    inline bool Open = false;
    inline bool FirstTime = true;
    inline ImFont* BiggestIcon = nullptr;
    inline ImFont* BiggestFont = nullptr;
    inline ImFont* littleFont = nullptr;
    inline int CurrentTab = 0;
    inline char AuthKey[128] = "";
    inline char ExecutorBuf[65536] = "";
    inline char ViewedScriptBuf[65536] = "";
    inline std::string ViewedScriptName;
    void Style();
    void Drawing();
    void ApplyRedEyedNinjaStyle();
}
