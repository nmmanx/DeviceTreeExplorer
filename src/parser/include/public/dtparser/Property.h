#ifndef PROPERTY_H
#define PROPERTY_H

#include <typeinfo>
#include <variant>

#include "dtparser/Node.h"
#include "dtparser/Common.h"
#include "dtparser/Element.h"

namespace dtparser {

class Node;

enum class PropertyType: uint8_t {
    UNKNOWN,
    EMPTY,
    U32,
    U64,
    PHANDLE,
    STRING,
    STRING_LIST,
    PROP_ENCODED_ARRAY,
};

struct Phandle {
    uint32_t id;
};

struct PropertyValue {
    std::string label;
    std::string raw;
    std::variant<std::string, uint32_t, Phandle> value;
};

class Property: public Element {
public:
    Property(
        const std::string &name,
        const std::string &label,
        const std::vector<PropertyValue>& values
    );

    PropertyType getType() const;
    bool isEmpty() const;

    std::vector<PropertyValue> getValues() const;

private:
    PropertyType m_type;
    std::vector<PropertyValue> m_values;
};

// Standard Property Subclasses

class CompatibleProperty : public Property {
public:
    CompatibleProperty(const std::string &label, const PropertyValue &value)
        : Property("compatible", label, { value }) {}

    CompatibleProperty(const PropertyValue &value)
        : CompatibleProperty("", value) {}
};

} // namespace dtparser

#endif // PROPERTY_H