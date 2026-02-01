#include "ServerInfo.hpp"
#include "d3d_Hook.hpp"

namespace ServerInfo {

std::string GetServerName() {
    if (!DirectX::Window) return "(No window)";
    wchar_t buf[512] = {};
    if (GetWindowTextW(DirectX::Window, buf, 511) == 0) return "(Loading...)";
    char narrow[512] = {};
    WideCharToMultiByte(CP_UTF8, 0, buf, -1, narrow, 511, NULL, NULL);
    std::string title(narrow);
    size_t pipe = title.find(" | ");
    if (pipe != std::string::npos)
        return title.substr(0, pipe);
    return title.empty() ? "(Not in server)" : title;
}

std::vector<std::string> GetResourceList() {
    std::vector<std::string> list;
    wchar_t path[MAX_PATH] = {};
    if (GetEnvironmentVariableW(L"LOCALAPPDATA", path, MAX_PATH) == 0) return list;
    std::wstring base(path);
    base += L"\\FiveM\\FiveM.app\\data\\nui-storage\\";
    
    WIN32_FIND_DATAW fd = {};
    std::wstring search = base + L"*";
    HANDLE h = FindFirstFileW(search.c_str(), &fd);
    if (h != INVALID_HANDLE_VALUE) {
        int count = 0;
        do {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..")) {
                    char narrow[260] = {};
                    WideCharToMultiByte(CP_UTF8, 0, fd.cFileName, -1, narrow, 259, NULL, NULL);
                    std::string s(narrow);
                    if (s.length() > 2) list.push_back(s);
                    if (++count >= 50) break;
                }
            }
        } while (FindNextFileW(h, &fd));
        FindClose(h);
    }
    return list;
}

}
