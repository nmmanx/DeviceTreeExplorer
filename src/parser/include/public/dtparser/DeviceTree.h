#ifndef DEVICETREE_H
#define DEVICETREE_H

#include "dtparser/Node.h"

namespace dtparser {

class DeviceTree
{
public:
    DeviceTree();

private:
    Node m_root;
};

} // namespace dtparser

#endif // DEVICETREE_H
