#ifndef NODE_H
#define NODE_H

#include "dtparser/Types.h"
#include "dtparser/Element.h"

namespace dtparser {

class Property;

class Node: public Element
{
public:
    bool isRoot() const;
    bool isDeleted() const;

    std::string getPath() const;
    std::string getUnitAddress() const;
    
    std::optional<sp<Property>> getProperty(const std::string &name) const;
    std::vector<sp<Property>> getProperties() const;
    std::vector<sp<Node>> getChildren() const;

    void dump(std::ostream &os, int indent = 0, bool verbose = false) const override;

private:
    Node(const std::string& fullName);

    void addChild(const sp<Node> &child);
    void addProperty(const sp<Property> &property);
    void setPath(const std::string &path);

    friend class Driver;

private:
    std::string m_path;
    std::string m_unitAddress;

    bool m_isRoot;

    std::vector<sp<Property>> m_properties;
    std::vector<sp<Node>> m_children;
};

} // namespace dtparser

#endif // NODE_H