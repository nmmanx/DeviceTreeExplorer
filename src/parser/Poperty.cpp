#include "dtparser/Property.h"

namespace dtparser {

Property::Property(
    const std::string &name,
    const std::string &label,
    const std::vector<PropertyValue> &values)
    : Element(name, label), m_type(PropertyType::UNKNOWN), m_values(values)
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

std::vector<PropertyValue> Property::getValues() const
{
    return m_values;
}

}; // namespace dtparser

