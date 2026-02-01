#pragma once

#include <Windows.h>
#include <string>
#include <vector>

namespace ServerInfo {
    std::string GetServerName();
    std::vector<std::string> GetResourceList();
}
