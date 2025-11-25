#ifndef DEVICETREE_H
#define DEVICETREE_H

#include "dtparser/Node.h"

namespace dtparser {

class DeviceTree
{
public:
    DeviceTree();

    void dump(std::ostream &os, bool verbose = false) const;

private:
    sp<Node> m_rootNode;
    friend class Driver;
};

} // namespace dtparser

#endif // DEVICETREE_H
