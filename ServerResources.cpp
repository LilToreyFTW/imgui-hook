#include "ServerResources.hpp"
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace ServerResources {

static std::string GetFiveMCachePath() {
    char path[MAX_PATH] = {};
    if (GetEnvironmentVariableA("LOCALAPPDATA", path, MAX_PATH) == 0) return "";
    return std::string(path) + "\\FiveM\\FiveM.app\\data\\";
}

static void ScanDirectoryRecursive(const std::string& dir, const std::string& relPath, ServerResource& out, int& fileCount) {
    if (fileCount >= 100) return;
    WIN32_FIND_DATAA fd = {};
    HANDLE h = FindFirstFileA((dir + "*").c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0) continue;
        std::string full = dir + fd.cFileName;
        std::string rel = relPath.empty() ? fd.cFileName : relPath + "\\" + fd.cFileName;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            ScanDirectoryRecursive(full + "\\", rel, out, fileCount);
        } else {
            ResourceFile rf;
            rf.path = rel;
            rf.isScript = (rel.find(".lua") != std::string::npos || rel.find(".js") != std::string::npos ||
                rel.find(".ts") != std::string::npos || rel.find(".html") != std::string::npos);
            out.files.push_back(rf);
            fileCount++;
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
}

std::vector<ServerResource> ScanFromCache() {
    std::vector<ServerResource> result;
    std::string base = GetFiveMCachePath();
    if (base.empty()) return result;

    std::string nuiPath = base + "nui-storage\\";
    WIN32_FIND_DATAA fd = {};
    HANDLE h = FindFirstFileA((nuiPath + "*").c_str(), &fd);
    if (h != INVALID_HANDLE_VALUE) {
        int count = 0;
        do {
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..")) {
                std::string name(fd.cFileName);
                if (name.length() > 2 && count < 40) {
                    ServerResource res;
                    res.name = name;
                    res.basePath = nuiPath + name;
                    int fc = 0;
                    ScanDirectoryRecursive(nuiPath + name + "\\", "", res, fc);
                    result.push_back(res);
                    count++;
                }
            }
        } while (FindNextFileA(h, &fd));
        FindClose(h);
    }

    return result;
}

std::string ReadFileContent(const std::string& resourcePath, const std::string& filePath) {
    std::ifstream f(resourcePath + "\\" + filePath, std::ios::binary);
    if (!f) return "";
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

}
