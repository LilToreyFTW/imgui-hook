#pragma once

#include <string>
#include <atomic>

namespace RedEyedNinjaAuth {
    void SetServer(const std::string& host, int port);
    bool Connect(const std::string& key);
    bool IsAuthenticated();
    std::string GetUsername();
    void SetOfflineBypass(bool bypass);
}
