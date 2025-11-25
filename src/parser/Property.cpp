#include "dtparser/Property.h"

namespace dtparser {

Property::Property(
    const std::string &name)
    : Element(name), m_type(PropertyType::UNKNOWN)
{
    // TODO: Determine property type based on values
}

PropertyType Property::getType() const
{
    return m_type;
}

bool Property::isEmpty() const
{
    return m_values.empty();
}

std::vector<PropertyValueType> Property::getValues() const
{
    return m_values;
}

}; // namespace dtparser

