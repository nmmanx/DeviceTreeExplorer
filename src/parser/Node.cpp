#include "dtparser/Node.h"

#include <iostream>

namespace dtparser {

Node::Node(
    const std::string &name,
    const std::string &unitAddress)
    : Element(name), m_unitAddress(unitAddress), m_isRoot(false)
{
    // TODO: Determina name and unit address
}

bool Node::isRoot() const
{
    return m_isRoot;
}

bool Node::isDeleted() const
{
    return m_isDeleted;
}

std::string Node::getPath() const
{
    return m_path;
}

std::string Node::getUnitAddress() const
{
    return m_unitAddress;
}

std::vector<std::shared_ptr<Property>> Node::getProperties() const
{
    return m_properties;
}

std::vector<std::shared_ptr<Node>> Node::getChildren() const
{
    return m_children;
}

void Node::dump(std::ostream &os, int indent, bool verbose) const
{
    auto unitAddressPart = m_unitAddress.empty() ? "" : "@" + m_unitAddress;
    std::string labelPart;

    if (auto label = getPrimaryLabel()) {
        labelPart = std::string(" (") + label->name + ")";
    }
    os << std::string(indent, ' ') << "+ " << getPath() << m_name << unitAddressPart << labelPart << std::endl;

    for (const auto &property : m_properties) {
        property->dump(os, indent + 4, verbose);
    }
    for (const auto &child : m_children) {
        child->dump(os, indent + 4, verbose);
    }
}

void Node::addChild(const sp<Node> &child)
{
    m_children.push_back(std::move(child));
}

void Node::addProperty(const sp<Property> &property)
{
    m_properties.push_back(property);
}

void Node::setPath(const std::string &path)
{
    m_path = path;
}

} // namespace dtparser