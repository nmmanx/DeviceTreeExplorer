#include "dtparser/Node.h"
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
    os << std::string(indent, ' ') << "- " << m_name << labelPart << std::endl;
    for (const auto &value : m_values) {
        if (verbose) {
            auto val = value2String(value);
            if (Property::isReference(value)) {
                auto ref = std::get<sp<Reference>>(value);
                auto targetNode = ref->getTargetNode();
                if (targetNode.has_value()) {
                    val += " -> " + targetNode.value().lock()->getPath();
                } else {
                    auto targetLabel = ref->getTargetLabel();
                    if (targetLabel.has_value()) {
                        val += " -> " + targetLabel.value().name;
                    } else {
                        val += " -> ?";
                    }
                }
            }
            os << std::string(indent + 4, ' ') << val << std::endl;
        }
    }
}

std::string Property::value2String(const PropertyValueType &value)
{
    return std::visit([](const auto &v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, PropertyValueU32>) {
            return std::to_string(v);
        } else if constexpr (std::is_same_v<T, sp<Reference>>) {
            return v->toString();
        } else if constexpr (std::is_same_v<T, PropertyValueByte>) {
            return std::to_string(v);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return "\"" + v + "\"";
        } else {
            return "unknown";
        }
    }, value);
}

bool Property::isU32(const PropertyValueType &value)
{
    return std::holds_alternative<PropertyValueU32>(value);
}

bool Property::isReference(const PropertyValueType &value)
{
    return std::holds_alternative<sp<Reference>>(value);
}

bool Property::isByte(const PropertyValueType &value)
{
    return std::holds_alternative<PropertyValueByte>(value);
}

bool Property::isString(const PropertyValueType &value)
{
    return std::holds_alternative<PropertyValueString>(value);
}

}; // namespace dtparser

