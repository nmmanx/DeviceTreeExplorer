#include "dtparser/Property.h"

#include <string>
#include <iostream>

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

size_t Property::getValuesCount() const
{
    return m_values.size();
}

void Property::addValue(const PropertyValueType &value)
{
    m_values.push_back(value);
}

void Property::dump(std::ostream &os, int indent, bool verbose) const
{
    std::string labelPart;

    if (auto label = getPrimaryLabel()) {
        labelPart = std::string("(") + label->name + ")";
    }
    os << std::string(indent, ' ') << "Property: " << m_name << labelPart << std::endl;
    for (const auto &value : m_values) {
        os << std::string(indent + 2, ' ') << value2String(value) << " ";
    }
    os << std::endl;
}

std::string Property::value2String(const PropertyValueType &value)
{
    return std::visit([](const auto &v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, PropertyValueU32>) {
            return u32ToString(v);
        } else if constexpr (std::is_same_v<T, PropertyValueByte>) {
            return std::to_string(v);
        } else if constexpr (std::is_same_v<T, sp<Reference>>) {
            return v->toString();
        } else if constexpr (std::is_same_v<T, std::string>) {
            return "\"" + v + "\"";
        } else {
            return "unknown";
        }
    }, value);
}

std::string Property::u32ToString(const PropertyValueU32 &value)
{
    return std::visit([](const auto &v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, uint32_t>) {
            return std::to_string(v);
        } else if constexpr (std::is_same_v<T, sp<Reference>>) {
            return v->toString();
        } else {
            return "unknown";
        }
    }, value);
}

}; // namespace dtparser

