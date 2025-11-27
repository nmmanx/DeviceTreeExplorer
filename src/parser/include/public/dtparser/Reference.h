#ifndef REFERENCE_H
#define REFERENCE_H

#include <functional>

#include "dtparser/Common.h"

namespace dtparser {

class Labelable;
class Node;

struct Label {
    std::string name;
    SourceLocation location;
    wp<Labelable> target;
    int position; // -1 means top-level label. Ex: label: somenode {}
};

typedef std::function<bool(const Label*, const sp<Node>&)> ReferenceResolver; 

class Labelable {
public:
    std::vector<Label> getLabels() const;
    std::optional<Label> getPrimaryLabel() const;

    void addLabel(const Label &label);

protected:
    std::vector<Label> m_labels;
};

class Reference {
public:
    Reference(const std::string &name, const SourceLocation &loc);

    std::string getName() const;
    SourceLocation getLocation() const;
    std::string toString() const;

    std::optional<Label> getTargetLabel() const;
    std::optional<wp<Node>> getTargetNode() const;

    ReferenceResolver getResolver() const;

private:
    std::string m_name;
    SourceLocation m_location;
    std::variant<std::monostate, Label, wp<Node>> m_target;
    ReferenceResolver m_resolver;
};

}

#endif
