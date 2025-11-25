#include "dtparser/DeviceTree.h"

namespace dtparser {

DeviceTree::DeviceTree()
    : m_rootNode(nullptr)
{
}

void DeviceTree::dump(std::ostream &os, bool verbose) const
{
    if (m_rootNode == nullptr) {
        return;
    }
    m_rootNode->dump(os, 0, verbose);
}

}
