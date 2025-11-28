#include "dtparser/Label.h"
#include "dtparser/Node.h"
#include "dtparser/Property.h"

namespace dtparser {

std::optional<wp<Node>> Label::getTargetNode() const
{
    auto nodeTarget = std::dynamic_pointer_cast<Node>(this->target.lock());
    if (nodeTarget) {
        return wp<Node>(nodeTarget);
    }
    return std::nullopt;
}

std::optional<wp<Property>> Label::getTargetProperty() const
{
    auto propertyTarget = std::dynamic_pointer_cast<Property>(this->target.lock());
    if (propertyTarget) {
        return wp<Property>(propertyTarget);
    }
    return std::nullopt;
}

std::optional<PropertyValueType> Label::getTargetValue() const
{
    auto propertyTarget = std::dynamic_pointer_cast<Property>(this->target.lock());
    if (propertyTarget && this->position >= 0 && this->position < propertyTarget->getValuesCount()) {
        return propertyTarget->getValues()[this->position];
    }
    return std::nullopt;
}

}