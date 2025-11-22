#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <cstdint>

#include "dtparser/Property.h"

namespace dtparser {

class Property;

class Node
{
public:
    Node(
        const std::string& path,
        const std::string& name,
        const std::string& unitAddress,
        const Node* parent);

private:
    void addChild(const Node& child);
    void setDeleted(bool deleted);

private:
    std::string m_path;
    std::string m_name;
    std::string m_unitAddress;
    std::vector<Node> m_children;

    bool m_deleted;
};

} // namespace dtparser

#endif // NODE_H