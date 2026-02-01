#pragma once

#include "imgui/imgui.h"
#include <tuple>
#include <string>

enum ImGuiToastType_ { ImGuiToastType_None, ImGuiToastType_Success, ImGuiToastType_Warning, ImGuiToastType_Error };

namespace ImGui {
    inline void InsertNotification(std::tuple<int, int, const char*> n) { (void)n; }
    inline void InsertNotification(std::tuple<int, int, std::string> n) { (void)n; }
    template<typename... Args>
    inline void InsertNotification(Args&&...) {}
}
