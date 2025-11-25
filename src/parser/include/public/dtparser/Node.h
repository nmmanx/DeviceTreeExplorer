#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <cstdint>

#include "dtparser/Common.h"
#include "dtparser/Element.h"
#include "dtparser/Property.h"

namespace dtparser {

class Driver;

class Node: public Element
{
public:
    Node(
        const std::string& path,
        const std::string& name,
        const std::string& unitAddress = "");

    bool isRoot() const;
    bool isDeleted() const;

    std::string getPath() const;
    std::string getUnitAddress() const;
    std::vector<sp<Property>> getProperties() const;
    std::vector<sp<Node>> getChildren() const;

    void dump(std::ostream &os, int indent = 0, bool verbose = false) const override;

private:
    void addChild(const sp<Node> &child);
    void addProperty(const sp<Property> &property);

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