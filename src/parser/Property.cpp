#include "dtparser/Node.h"
#include "dtparser/Property.h"
#include "dtparser/Reference.h"

#include <string>
#include <iostream>

namespace dtparser {

Property::Property(const std::string &name)
    : Element(name)
{
}

bool Property::isEmpty() const
{
    return m_values.empty();
}

std::vector<PropertyValueType> Property::getValues() const
{
    return m_values;
}

std::optional<uint32_t> Property::getAsU32() const
{
    if (m_values.size() != 1 || !isU32(m_values[0])) {
        return std::nullopt;
    }
    return std::get<uint32_t>(m_values[0]);
}

std::optional<sp<Reference>> Property::getAsReference() const
{
    if (m_values.size() != 1 || !isReference(m_values[0])) {
        return std::nullopt;
    }
    return std::get<sp<Reference>>(m_values[0]);
}

std::optional<std::string> Property::getAsString() const
{
    if (m_values.size() != 1 || !isString(m_values[0])) {
        return std::nullopt;
    }
    return std::get<std::string>(m_values[0]);
}

std::optional<uint8_t> Property::getAsByte() const
{
    if (m_values.size() != 1 || !isByte(m_values[0])) {
        return std::nullopt;
    }
    return std::get<uint8_t>(m_values[0]);
}

std::optional<std::vector<std::string>> Property::getAsStringList() const
{
    std::vector<std::string> result;
    for (const auto &value : m_values) {
        if (isString(value)) {
            result.push_back(std::get<std::string>(value));
        } else {
            return std::nullopt;
        }
    }
    return result;
}

std::optional<std::vector<uint32_t>> Property::getAsU32Array() const
{
    std::vector<uint32_t> result;
    for (const auto &value : m_values) {
        if (isU32(value)) {
            result.push_back(std::get<uint32_t>(value));
        } else {
            return std::nullopt;
        }
    }
    return result;
}

std::optional<std::vector<uint8_t>> Property::getAsByteString() const
{
    std::vector<uint8_t> result;
    for (const auto &value : m_values) {
        if (isByte(value)) {
            result.push_back(std::get<uint8_t>(value));
        } else {
            return std::nullopt;
        }
    }
    return result;
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

