#include "dtparser/DeviceTreeSource.h"
#include "Driver.h"

#include <fstream>

namespace dtparser {

DeviceTreeSource::DeviceTreeSource(const fs::path& filePath, std::vector<fs::path> searchPaths)
    : m_filePath(filePath), m_searchPaths(searchPaths)
{

}

DeviceTreeSource::DeviceTreeSource(const fs::path& filePath)
    : DeviceTreeSource(filePath, {})
{

}

std::pair<ParseResult, sp<DeviceTree>> DeviceTreeSource::parse()
{
    sp<DeviceTree> dt(new DeviceTree());
    std::ofstream parserLogFile("./parser.log", std::ios::app);
    Driver drv(parserLogFile);

    auto parseResult = drv.parse(m_filePath.string().c_str(), dt.get());

    return std::make_pair(parseResult, dt);
}

}
