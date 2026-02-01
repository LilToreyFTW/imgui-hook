/*
 * RedEyedNinja-X Loader
 * Dark UI with red accents - similar to login panel style
 */

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <TlHelp32.h>
#include <Shellapi.h>
#include <WinHttp.h>
#include <string>
#include <vector>
#include <Shlwapi.h>
#include <CommCtrl.h>
#include <thread>
#include <dwmapi.h>
#include <cstring>
#include <cstdlib>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "dwmapi.lib")

namespace {

const wchar_t* DLL_NAME = L"cheese.dll";
const wchar_t* CRED_FILE = L"credentials.json";

LRESULT CALLBACK BtnSubclassProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

#define LOCAL_DEV 0

std::wstring g_panelHost = L"red-eyed-ninja-loader.vercel.app";
int g_panelPort = 443;
bool g_useHttps = true;

std::wstring GetLoaderDirectory();

void LoadPanelConfig() {
    std::wstring cfgPath = GetLoaderDirectory() + L"panel_url.txt";
    HANDLE h = CreateFileW(cfgPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (h != INVALID_HANDLE_VALUE) {
        char buf[256] = {};
        DWORD read = 0;
        if (ReadFile(h, buf, sizeof(buf) - 1, &read, NULL) && read > 0) {
            buf[read] = 0;
            std::string line(buf);
            while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
            std::wstring wline(line.begin(), line.end());
            if (wline.find(L"https://") == 0) {
                wline = wline.substr(8);
                g_useHttps = true;
                g_panelPort = 443;
            } else if (wline.find(L"http://") == 0) {
                wline = wline.substr(7);
                g_useHttps = false;
                g_panelPort = 80;
            }
            size_t slash = wline.find(L'/');
            if (slash != std::wstring::npos) wline = wline.substr(0, slash);
            size_t colon = wline.find(L':');
            if (colon != std::wstring::npos) {
                g_panelHost = wline.substr(0, colon);
                g_panelPort = _wtoi(wline.substr(colon + 1).c_str());
            } else {
                g_panelHost = wline;
            }
        }
        CloseHandle(h);
    }
#if LOCAL_DEV
    g_panelHost = L"localhost";
    g_panelPort = 3000;
    g_useHttps = false;
#endif
}
const wchar_t* PANEL_PATH = L"/api/validate";
const wchar_t* PING_PATH = L"/api/ping";

#define PANEL_HOST_STR() (g_panelHost.c_str())
#define PANEL_PORT_VAL() (g_panelPort)
#define USE_HTTPS_VAL() (g_useHttps)

const COLORREF CLR_BG = 0x0a0a0a;
const COLORREF CLR_INPUT_BG = 0x1a1a1a;
const COLORREF CLR_ACCENT = 0x005c5ccd;
const COLORREF CLR_TEXT = 0xededed;

HBRUSH g_hBrushBg = NULL;
HBRUSH g_hBrushInput = NULL;
HBRUSH g_hBrushAccent = NULL;

HWND g_hWnd = NULL;
HWND g_hPingText = NULL;
HWND g_hKeyEdit = NULL;
HWND g_hInjectBtn = NULL;
HWND g_hRegisterBtn = NULL;
HWND g_hStatusText = NULL;
HWND g_hCloseBtn = NULL;
HWND g_hInjectOnlyBtn = NULL;
HWND g_hTitleStatic = NULL;
HWND g_hKeyLabel = NULL;
std::wstring g_validatedKey;
bool g_monitoring = false;
bool g_injected = false;
bool g_loggedIn = false;

std::wstring GetLoaderDirectory() {
    wchar_t path[MAX_PATH] = {};
    if (GetModuleFileNameW(NULL, path, MAX_PATH) == 0) return L"";
    PathRemoveFileSpecW(path);
    return std::wstring(path) + L"\\";
}

DWORD FindFiveMProcess() {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return 0;
    PROCESSENTRY32W pe = { sizeof(pe) };
    DWORD pid = 0;
    if (Process32FirstW(snap, &pe)) {
        do {
            std::wstring name = pe.szExeFile;
            if (name.find(L"FiveM") != std::wstring::npos &&
                (name.find(L"GTAProcess") != std::wstring::npos || name.find(L"GameProcess") != std::wstring::npos)) {
                pid = pe.th32ProcessID;
                break;
            }
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);
    return pid;
}

bool InjectDLL(DWORD pid, const std::wstring& dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) return false;
    size_t pathSize = (dllPath.length() + 1) * sizeof(wchar_t);
    LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, pathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMem) { CloseHandle(hProcess); return false; }
    if (!WriteProcessMemory(hProcess, remoteMem, dllPath.c_str(), pathSize, NULL)) {
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    HMODULE hKernel = GetModuleHandleW(L"kernel32.dll");
    FARPROC loadLib = GetProcAddress(hKernel, "LoadLibraryW");
    if (!loadLib) {
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLib, remoteMem, 0, NULL);
    if (!hThread) {
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    WaitForSingleObject(hThread, 10000);
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return true;
}

std::string WideToUtf8(const std::wstring& w) {
    if (w.empty()) return "";
    int n = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), NULL, 0, NULL, NULL);
    std::string s(n, 0);
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), &s[0], n, NULL, NULL);
    return s;
}

std::string EscapeJson(const std::string& s) {
    std::string r;
    for (char c : s) {
        if (c == '"') r += "\\\"";
        else if (c == '\\') r += "\\\\";
        else if (c >= 32 && c < 127) r += c;
    }
    return r;
}

void SaveCredentials(const std::wstring& key) {
    std::string keyUtf8 = EscapeJson(WideToUtf8(key));
    std::string json = "{\"key\":\"" + keyUtf8 + "\"}";
    std::wstring path = GetLoaderDirectory() + CRED_FILE;
    HANDLE h = CreateFileW(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) {
        WriteFile(h, json.c_str(), (DWORD)json.size(), NULL, NULL);
        CloseHandle(h);
    }
}

bool LoadCredentials(std::wstring* outKey) {
    std::wstring path = GetLoaderDirectory() + CRED_FILE;
    HANDLE h = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return false;
    char buf[512] = {};
    DWORD read = 0;
    if (!ReadFile(h, buf, sizeof(buf) - 1, &read, NULL)) { CloseHandle(h); return false; }
    CloseHandle(h);
    buf[read] = 0;
    const char* keyStart = strstr(buf, "\"key\":\"");
    if (!keyStart) return false;
    keyStart += 7;
    std::string keyStr;
    for (const char* p = keyStart; *p && keyStr.size() < 120; p++) {
        if (*p == '"') break;
        if (*p == '\\' && (p[1] == '"' || p[1] == '\\')) { keyStr += p[1]; p++; continue; }
        keyStr += *p;
    }
    std::wstring key(keyStr.begin(), keyStr.end());
    *outKey = key;
    return !key.empty();
}

bool HasNoCopyArg() {
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!argv) return false;
    for (int i = 1; i < argc; i++) {
        if (_wcsicmp(argv[i], L"--no-copy") == 0) { LocalFree(argv); return true; }
    }
    LocalFree(argv);
    return false;
}

bool RelaunchWithRandomName() {
    wchar_t selfPath[MAX_PATH] = {};
    if (GetModuleFileNameW(NULL, selfPath, MAX_PATH) == 0) return false;
    std::wstring dir = GetLoaderDirectory();
    srand((unsigned)GetTickCount());
    wchar_t newName[64];
    swprintf_s(newName, L"Runtime_%04X%04X.exe", rand() & 0xFFFF, rand() & 0xFFFF);
    std::wstring newPath = dir + newName;
    if (!CopyFileW(selfPath, newPath.c_str(), FALSE)) return false;
    std::wstring cmdLine = L"\"" + newPath + L"\" --no-copy";
    std::vector<wchar_t> cmdBuf(cmdLine.begin(), cmdLine.end());
    cmdBuf.push_back(0);
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    if (!CreateProcessW(newPath.c_str(), cmdBuf.data(), NULL, NULL, FALSE, 0, NULL, dir.c_str(), &si, &pi))
        return false;
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return true;
}

std::string GetHWID() {
    DWORD volSerial = 0;
    wchar_t compName[MAX_COMPUTERNAME_LENGTH + 1] = {};
    DWORD compLen = MAX_COMPUTERNAME_LENGTH + 1;
    GetVolumeInformationW(L"C:\\", NULL, 0, &volSerial, NULL, NULL, NULL, 0);
    GetComputerNameW(compName, &compLen);
    std::string seed = std::to_string(volSerial) + WideToUtf8(compName);
    unsigned long hash = 5381;
    for (char c : seed) hash = ((hash << 5) + hash) + (unsigned char)c;
    char buf[32];
    sprintf_s(buf, "%08lx%08lx", hash, hash ^ 0x5a5a5a5a);
    return std::string(buf);
}

int MeasurePing() {
    DWORD t0 = GetTickCount();
    HINTERNET hSession = WinHttpOpen(L"RedEyedNinja-Loader/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) return -1;
    HINTERNET hConnect = WinHttpConnect(hSession, PANEL_HOST_STR(), PANEL_PORT_VAL(), 0);
    if (!hConnect) { WinHttpCloseHandle(hSession); return -1; }
    DWORD flags = USE_HTTPS_VAL() ? WINHTTP_FLAG_SECURE : 0;
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", PING_PATH, NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES, flags);
    if (!hRequest) { WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return -1; }
    if (USE_HTTPS_VAL()) {
        DWORD secFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
        WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &secFlags, sizeof(secFlags));
    }
    BOOL r = WinHttpSendRequest(hRequest, NULL, 0, NULL, 0, 0, 0);
    if (!r) { WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return -1; }
    r = WinHttpReceiveResponse(hRequest, NULL);
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return r ? (int)(GetTickCount() - t0) : -1;
}

bool ValidateKey(const std::wstring& key, std::string* outMsg = nullptr) {
    std::string keyUtf8 = WideToUtf8(key);
    std::string hwid = GetHWID();
    std::string body = "{\"key\":\"" + EscapeJson(keyUtf8) + "\",\"hwid\":\"" + EscapeJson(hwid) + "\"}";
    HINTERNET hSession = WinHttpOpen(L"RedEyedNinja-Loader/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) return false;
    HINTERNET hConnect = WinHttpConnect(hSession, PANEL_HOST_STR(), PANEL_PORT_VAL(), 0);
    if (!hConnect) { WinHttpCloseHandle(hSession); return false; }
    DWORD flags = USE_HTTPS_VAL() ? WINHTTP_FLAG_SECURE : 0;
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", PANEL_PATH, NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES, flags);
    if (!hRequest) { WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return false; }
    if (USE_HTTPS_VAL()) {
        DWORD secFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
        WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &secFlags, sizeof(secFlags));
    }
    std::wstring headers = L"Content-Type: application/json\r\n";
    BOOL r = WinHttpSendRequest(hRequest, headers.c_str(), (DWORD)headers.size(),
        (LPVOID)body.c_str(), (DWORD)body.size(), (DWORD)body.size(), 0);
    if (!r) { WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return false; }
    r = WinHttpReceiveResponse(hRequest, NULL);
    if (!r) { WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return false; }
    char buf[512] = {};
    DWORD len = sizeof(buf) - 1;
    r = WinHttpReadData(hRequest, buf, len, &len);
    buf[len] = 0;
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    if (!r) return false;
    if (strstr(buf, "\"valid\":true") != NULL) return true;
    if (outMsg && strstr(buf, "\"message\":\"") != NULL) {
        const char* p = strstr(buf, "\"message\":\"") + 11;
        const char* e = strchr(p, '"');
        if (e && e - p < 64) {
            char msg[65] = {};
            memcpy(msg, p, (size_t)(e - p));
            *outMsg = msg;
        }
    }
    return false;
}

void MonitorThread() {
    std::wstring dllPath = GetLoaderDirectory() + DLL_NAME;
    if (GetFileAttributesW(dllPath.c_str()) == INVALID_FILE_ATTRIBUTES) return;
    for (int i = 0; i < 300 && g_monitoring; i++) {
        DWORD pid = FindFiveMProcess();
        if (pid && !g_injected) {
            if (InjectDLL(pid, dllPath)) {
                g_injected = true;
                if (g_hStatusText) SetWindowTextW(g_hStatusText, L"Injected! Press INSERT or F5 in-game.");
                if (g_hInjectOnlyBtn) EnableWindow(g_hInjectOnlyBtn, FALSE);
                MessageBoxW(g_hWnd, L"Press OK when you are loaded into a FiveM server.",
                    L"RedEyedNinja-X", MB_OK | MB_ICONINFORMATION);
                break;
            }
        }
        Sleep(1000);
    }
    g_monitoring = false;
}

void DoInject() {
    std::wstring dllPath = GetLoaderDirectory() + DLL_NAME;
    if (GetFileAttributesW(dllPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        SetWindowTextW(g_hStatusText, L"DLL not found. Place cheese.dll in loader folder.");
        return;
    }
    DWORD pid = FindFiveMProcess();
    if (pid) {
        if (InjectDLL(pid, dllPath)) {
            g_injected = true;
            SetWindowTextW(g_hStatusText, L"Injection successful! Press INSERT or F5 in-game.");
            if (g_hInjectOnlyBtn) EnableWindow(g_hInjectOnlyBtn, FALSE);
            MessageBoxW(g_hWnd, L"Press OK when you are loaded into a FiveM server.",
                L"RedEyedNinja-X", MB_OK | MB_ICONINFORMATION);
            return;
        }
    }
    SetWindowTextW(g_hStatusText, L"Injection ready. Start FiveM - will auto-inject when detected.");
    g_monitoring = true;
    std::thread(MonitorThread).detach();
}

void OnLogin() {
    wchar_t keyBuf[128] = {};
    GetWindowTextW(g_hKeyEdit, keyBuf, 127);
    std::wstring key(keyBuf);
    while (!key.empty() && key.back() == L' ') key.pop_back();
    while (!key.empty() && key.front() == L' ') key.erase(0, 1);
    if (key.empty()) {
        SetWindowTextW(g_hStatusText, L"Enter your subscription key from the panel.");
        return;
    }
    SetWindowTextW(g_hStatusText, L"Validating key...");
    UpdateWindow(g_hWnd);
    std::string errMsg;
    if (!ValidateKey(key, &errMsg)) {
        std::wstring msg = L"Invalid key. Register at panel and get a valid key.";
        if (!errMsg.empty()) {
            msg = std::wstring(errMsg.begin(), errMsg.end());
        }
        SetWindowTextW(g_hStatusText, msg.c_str());
        return;
    }
    g_validatedKey = key;
    g_loggedIn = true;
    SaveCredentials(key);
    ShowWindow(g_hKeyEdit, SW_HIDE);
    if (g_hKeyLabel) ShowWindow(g_hKeyLabel, SW_HIDE);
    ShowWindow(g_hInjectBtn, SW_HIDE);
    ShowWindow(g_hRegisterBtn, SW_HIDE);
    if (g_hTitleStatic) { SetWindowTextW(g_hTitleStatic, L"Inject"); InvalidateRect(g_hTitleStatic, NULL, TRUE); }
    g_hInjectOnlyBtn = CreateWindowW(L"BUTTON", L"Inject",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        24, 120, 364, 48, g_hWnd, (HMENU)4, NULL, NULL);
    SetWindowSubclass(g_hInjectOnlyBtn, BtnSubclassProc, 0, 0);
    SetWindowTextW(g_hStatusText, L"Key verified. Click Inject to inject into FiveM.");
    DoInject();
}

void OnInjectClick() {
    DoInject();
}

void TryAutoLogin() {
    std::wstring key;
    if (!LoadCredentials(&key)) return;
    if (key.empty()) return;
    std::string errMsg;
    if (!ValidateKey(key, &errMsg)) return;
    g_validatedKey = key;
    g_loggedIn = true;
    ShowWindow(g_hKeyEdit, SW_HIDE);
    if (g_hKeyLabel) ShowWindow(g_hKeyLabel, SW_HIDE);
    ShowWindow(g_hInjectBtn, SW_HIDE);
    ShowWindow(g_hRegisterBtn, SW_HIDE);
    if (g_hTitleStatic) { SetWindowTextW(g_hTitleStatic, L"Inject"); InvalidateRect(g_hTitleStatic, NULL, TRUE); }
    g_hInjectOnlyBtn = CreateWindowW(L"BUTTON", L"Inject",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        24, 120, 364, 48, g_hWnd, (HMENU)4, NULL, NULL);
    SetWindowSubclass(g_hInjectOnlyBtn, BtnSubclassProc, 0, 0);
    SetWindowTextW(g_hStatusText, L"Auto-logged in. Click Inject to inject into FiveM.");
}

void OpenPanel() {
    std::wstring url = USE_HTTPS_VAL() ? L"https://" : L"http://";
    url += g_panelHost;
    ShellExecuteW(NULL, L"open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void UpdatePingDisplay() {
    if (!g_hPingText) return;
    int ping = MeasurePing();
    wchar_t buf[64];
    if (ping >= 0)
        swprintf_s(buf, L"\x2022 %d ms ping", ping);
    else
        swprintf_s(buf, L"\x2022 No connection");
    SetWindowTextW(g_hPingText, buf);
}

LRESULT CALLBACK BtnSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
    if (msg == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
        bool hover = (GetCapture() == hWnd);
        bool pressed = (GetKeyState(VK_LBUTTON) & 0x8000) && hover;
        HBRUSH br = CreateSolidBrush(pressed ? 0x004040a0 : (hover ? 0x007070d8 : CLR_ACCENT));
        FillRect(hdc, &rc, br);
        DeleteObject(br);
        wchar_t text[64];
        GetWindowTextW(hWnd, text, 63);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, 0xffffff);
        DrawTextW(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        EndPaint(hWnd, &ps);
        return 0;
    }
    if (msg == WM_MOUSEMOVE) {
        TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, hWnd, 0 };
        TrackMouseEvent(&tme);
        InvalidateRect(hWnd, NULL, TRUE);
    }
    if (msg == WM_MOUSELEAVE) InvalidateRect(hWnd, NULL, TRUE);
    return DefSubclassProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        if (!g_hBrushInput) g_hBrushInput = CreateSolidBrush(CLR_INPUT_BG);
        if (!g_hBrushAccent) g_hBrushAccent = CreateSolidBrush(CLR_ACCENT);

        g_hPingText = CreateWindowW(L"STATIC", L"\x2022 Checking...",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            24, 24, 200, 20, hWnd, NULL, NULL, NULL);

        g_hCloseBtn = CreateWindowW(L"BUTTON", L"X",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            356, 16, 36, 28, hWnd, (HMENU)3, NULL, NULL);

        g_hTitleStatic = CreateWindowW(L"STATIC", L"Log in",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            0, 56, 420, 32, hWnd, NULL, NULL, NULL);

        g_hKeyLabel = CreateWindowW(L"STATIC", L"Subscription Key",
            WS_CHILD | WS_VISIBLE,
            24, 108, 200, 20, hWnd, NULL, NULL, NULL);

        g_hKeyEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            24, 132, 364, 36, hWnd, NULL, NULL, NULL);

        g_hInjectBtn = CreateWindowW(L"BUTTON", L"Login",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            24, 188, 364, 40, hWnd, (HMENU)1, NULL, NULL);

        g_hRegisterBtn = CreateWindowW(L"BUTTON", L"Register",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            24, 240, 364, 40, hWnd, (HMENU)2, NULL, NULL);

        g_hStatusText = CreateWindowW(L"STATIC", L"",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            24, 292, 364, 40, hWnd, NULL, NULL, NULL);

        SetWindowSubclass(g_hInjectBtn, BtnSubclassProc, 0, 0);
        SetWindowSubclass(g_hRegisterBtn, BtnSubclassProc, 0, 0);

        SetTimer(hWnd, 1, 5000, NULL);
        PostMessage(hWnd, WM_USER + 2, 0, 0);
        HWND hw = hWnd;
        std::thread([hw]() { Sleep(500); PostMessage(hw, WM_USER + 1, 0, 0); }).detach();
        break;
    }
    case WM_USER + 2:
        TryAutoLogin();
        break;
    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, CLR_TEXT);
        SetBkMode(hdc, TRANSPARENT);
        return (LRESULT)g_hBrushBg;
    }
    case WM_CTLCOLOREDIT: {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, CLR_TEXT);
        SetBkColor(hdc, CLR_INPUT_BG);
        return (LRESULT)g_hBrushInput;
    }
    case WM_CTLCOLORBTN: {
        return (LRESULT)g_hBrushBg;
    }
    case WM_USER + 1:
        UpdatePingDisplay();
        break;
    case WM_TIMER:
        if (wParam == 1) UpdatePingDisplay();
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) OnLogin();
        else if (LOWORD(wParam) == 2) OpenPanel();
        else if (LOWORD(wParam) == 3) DestroyWindow(hWnd);
        else if (LOWORD(wParam) == 4) OnInjectClick();
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        g_monitoring = false;
        if (g_hBrushBg) DeleteObject(g_hBrushBg);
        if (g_hBrushInput) DeleteObject(g_hBrushInput);
        if (g_hBrushAccent) DeleteObject(g_hBrushAccent);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }
    return 0;
}

} // namespace

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int) {
    if (!HasNoCopyArg()) {
        if (RelaunchWithRandomName()) return 0;
    }
    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_STANDARD_CLASSES };
    InitCommonControlsEx(&icc);
    LoadPanelConfig();

    std::wstring dllPath = GetLoaderDirectory() + DLL_NAME;
    if (GetFileAttributesW(dllPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        MessageBoxW(NULL, (L"cheese.dll not found in:\n" + dllPath).c_str(), L"Loader", MB_OK | MB_ICONERROR);
        return 1;
    }

    g_hBrushBg = CreateSolidBrush(CLR_BG);

    WNDCLASSEXW wc = { sizeof(wc) };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = g_hBrushBg;
    wc.lpszClassName = L"RedEyedNinjaLoader";
    if (!RegisterClassExW(&wc)) return 1;

    g_hWnd = CreateWindowExW(0, wc.lpszClassName, L"RedEyedNinja-X",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 420, 380, NULL, NULL, hInst, NULL);
    if (!g_hWnd) return 1;

    SetWindowLong(g_hWnd, GWL_STYLE, GetWindowLong(g_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);

    MARGINS m = { 0, 0, 1, 0 };
    DwmExtendFrameIntoClientArea(g_hWnd, &m);

    ShowWindow(g_hWnd, SW_SHOW);

    MSG m2;
    while (GetMessage(&m2, NULL, 0, 0)) {
        TranslateMessage(&m2);
        DispatchMessage(&m2);
    }
    return (int)m2.wParam;
}
