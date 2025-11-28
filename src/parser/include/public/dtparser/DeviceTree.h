#ifndef DEVICETREE_H
#define DEVICETREE_H

#include "dtparser/Node.h"

namespace dtparser {

class DeviceTree
{
public:
    DeviceTree(const DeviceTree& other) = delete;
    DeviceTree& operator=(const DeviceTree& other) = delete;

    // Allow transfering ownership
    DeviceTree(DeviceTree&& other) noexcept = default;
    DeviceTree& operator=(DeviceTree&& other) noexcept = default;

    sp<Node> getRootNode() const { return m_rootNode; }

    void dump(std::ostream &os, bool verbose = false) const;

private:
    DeviceTree();

    friend class DeviceTreeSource;

private:
    sp<Node> m_rootNode;
    friend class Driver;
};

} // namespace dtparser

#endif // DEVICETREE_H
