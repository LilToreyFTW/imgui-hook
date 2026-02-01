#pragma once

#include <string>
#include <vector>
#include <map>

struct ResourceFile {
    std::string path;
    std::string content;
    bool isScript;
};

struct ServerResource {
    std::string name;
    std::string basePath;
    std::vector<ResourceFile> files;
};

namespace ServerResources {
    std::vector<ServerResource> ScanFromCache();
    std::string ReadFileContent(const std::string& resourcePath, const std::string& filePath);
}
