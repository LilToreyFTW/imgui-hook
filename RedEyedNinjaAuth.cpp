#include "RedEyedNinjaAuth.hpp"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <thread>
#include <sstream>
#pragma comment(lib, "ws2_32.lib")

namespace RedEyedNinjaAuth {
    static std::string g_host = "127.0.0.1";
    static int g_port = 6969;
    static std::atomic<bool> g_authenticated{ false };
    static std::string g_username = "User";
    static std::atomic<bool> g_offlineBypass{ false };

    void SetServer(const std::string& host, int port) {
        g_host = host;
        g_port = port;
    }

    void SetOfflineBypass(bool bypass) { g_offlineBypass = bypass; }

    bool Connect(const std::string& key) {
        if (g_offlineBypass) {
            g_authenticated = true;
            g_username = "Offline";
            return true;
        }
        WSADATA wsa = {};
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return false;

        SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
        if (s == INVALID_SOCKET) { WSACleanup(); return false; }

        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons((u_short)g_port);
        inet_pton(AF_INET, g_host.c_str(), &addr.sin_addr);

        bool ok = (connect(s, (sockaddr*)&addr, sizeof(addr)) == 0);
        if (ok) {
            std::string msg = "AUTH:" + key + "\n";
            send(s, msg.c_str(), (int)msg.size(), 0);
            char buf[512] = {};
            int n = recv(s, buf, sizeof(buf) - 1, 0);
            if (n > 0) {
                buf[n] = 0;
                std::string resp(buf);
                if (resp.find("OK") != std::string::npos || resp.find("success") != std::string::npos) {
                    g_authenticated = true;
                    size_t u = resp.find("user:");
                    if (u != std::string::npos)
                        g_username = resp.substr(u + 5, resp.find('\n', u) - u - 5);
                }
            }
            closesocket(s);
        }
        WSACleanup();
        return g_authenticated;
    }

    bool IsAuthenticated() { return g_authenticated || g_offlineBypass; }
    std::string GetUsername() { return g_username; }
}
