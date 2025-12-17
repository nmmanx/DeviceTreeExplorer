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

std::pair<ParseResult, up<DeviceTree>> DeviceTreeSource::parse()
{
    DeviceTree* dt = new DeviceTree();

    std::ofstream parserLogFile("./parser.log", std::ios::app);
    Driver drv(parserLogFile);

    auto parseResult = drv.parse(m_filePath.string().c_str(), dt);

    return std::make_pair(parseResult, std::unique_ptr<DeviceTree>(dt));
}

fs::path DeviceTreeSource::getFilePath() const
{
    return m_filePath;
}

}
