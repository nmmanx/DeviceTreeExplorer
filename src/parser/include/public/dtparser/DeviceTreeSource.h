#ifndef DEVICETREESOURCE_H
#define DEVICETREESOURCE_H

#include <filesystem>
#include <vector>
#include <utility>

#include "dtparser/DeviceTree.h"

namespace fs = std::filesystem;

namespace dtparser {

struct ParseResult {
    bool success;
    std::string errorMessage;
    SourceLocation errorLocation;
};

class DeviceTreeSource
{
public:
    DeviceTreeSource(const fs::path& filePath, std::vector<fs::path> searchPaths);
    DeviceTreeSource(const fs::path& filePath);

    std::pair<ParseResult, sp<DeviceTree>> parse();

private:
    fs::path m_filePath;
    std::vector<fs::path> m_searchPaths;
};

} // namespace dtparser

#endif // DEVICETREESOURCE_H
