#ifndef DEVICETREESOURCE_H
#define DEVICETREESOURCE_H

#include <filesystem>
#include <vector>

#include "DeviceTree.h"

namespace fs = std::filesystem;

namespace dtparser {

struct ParseResult {
    bool success;
};

class DeviceTreeSource
{
public:
    DeviceTreeSource(const fs::path& filePath, std::vector<fs::path> searchPaths);

    ParseResult parse(DeviceTree *dt);

private:
    fs::path m_filePath;
    std::vector<fs::path> m_searchPaths;
};

} // namespace dtparser

#endif // DEVICETREESOURCE_H
