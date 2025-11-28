#ifndef REFERENCE_H
#define REFERENCE_H

#include <functional>
#include <optional>

#include "dtparser/Types.h"
#include "dtparser/Label.h"

namespace dtparser {

class Label;
class Node;

using ReferenceResolver = std::function<bool(const Label*, const sp<Node>&)>;

class Reference {
public:
    std::string getName() const;
    SourceLocation getLocation() const;
    std::string toString() const;

    // Use this in case the reference is pointing to a label
    std::optional<Label> getTargetLabel() const;

    // Use this in case the reference is pointing to a node by explicit path (e.g. "&{/parent/node@0}")
    std::optional<wp<Node>> getTargetNode() const;

private:
    Reference(const std::string &name, const SourceLocation &loc);

    ReferenceResolver getResolver() const;

    friend class Driver;

private:
    std::string m_name;
    SourceLocation m_location;
    std::variant<std::monostate, Label, wp<Node>> m_target;
    ReferenceResolver m_resolver;
};

}

#endif
