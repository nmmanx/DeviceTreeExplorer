#include "dtparser/Node.h"

namespace dtparser {

Node::Node(
    const std::string &path,
    const std::string &name,
    const std::string &label,
    const std::string &unitAddress)
    : Element(name, label), m_path(path), m_unitAddress(unitAddress), m_isRoot(false), m_parent(nullptr)
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

std::shared_ptr<Node> Node::getParent() const
{
    return m_parent;
}

std::vector<std::shared_ptr<Property>> Node::getProperties() const
{
    return m_properties;
}

std::vector<std::shared_ptr<Node>> Node::getChildren() const
{
    return m_children;
}

void Node::setParent(const sp<Node> &parent)
{
    m_parent = parent;
}

void Node::addChild(const sp<Node> &child)
{
    m_children.push_back(std::move(child));
}

void Node::addProperty(const sp<Property> &property)
{
    m_properties.push_back(std::make_shared<Property>(property));
}

} // namespace dtparser