#include "dtparser/DeviceTreeSource.h"
#include "Driver.h"

namespace dtparser {

DeviceTreeSource::DeviceTreeSource(const fs::path& filePath, std::vector<fs::path> searchPaths)
    : m_filePath(filePath), m_searchPaths(searchPaths)
{

}

DeviceTreeSource::DeviceTreeSource(const fs::path& filePath)
    : DeviceTreeSource(filePath, {})
{

}

ParseResult DeviceTreeSource::parse(DeviceTree *dt)
{
    Driver drv;
    return drv.parse(m_filePath.string().c_str(), dt);
}

}
