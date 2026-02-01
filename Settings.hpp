#pragma once

namespace Settings {
    namespace Player {
        inline bool NoClip = false;
    }
    namespace Aimbot {
        inline bool Aimbot = false;
        inline bool silentaim = false;
        inline bool aimmousewhilesilent = false;
        inline bool Draw_Fov = false;
        inline bool crosshair = false;
        inline float AimbotFov = 100.f;
        inline float AimbotSmooth = 5.f;
        inline int AimbotBone = 0;
        inline int AimbotTypes = 0;
        inline int TypeOfTarget = 0;
        inline float DistanceAimbotLimit = 200.f;
        inline int Hotkey = VK_RBUTTON;
        inline bool magic = false;
        inline bool Check_Fov = false;
    }
}
