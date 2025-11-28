#ifndef LABEL_H
#define LABEL_H

#include <optional>

#include "dtparser/Types.h"

namespace dtparser {

class Label;
class Node;
class Property;

class Labelable {
public:
    virtual ~Labelable() {}

    std::vector<Label> getLabels() const;
    std::optional<Label> getPrimaryLabel() const;

    void addLabel(const Label &label);

protected:
    std::vector<Label> m_labels;
};

struct Label {
    std::string name;
    SourceLocation location;
    wp<Labelable> target;
    int position; // -1 means top-level label. Ex: label: somenode {}

    std::optional<wp<Node>> getTargetNode() const;
    std::optional<wp<Property>> getTargetProperty() const;
    std::optional<PropertyValueType> getTargetValue() const;
};

}

#endif