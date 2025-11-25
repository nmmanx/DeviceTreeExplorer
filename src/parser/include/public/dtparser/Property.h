#ifndef PROPERTY_H
#define PROPERTY_H

#include <typeinfo>
#include <variant>

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

typedef uint8_t PropertyValueByte;
typedef std::variant<uint32_t, sp<Reference>> PropertyValueU32;
typedef std::string PropertyValueString;

typedef std::vector<PropertyValueU32> PropertyValueU32Array;
typedef std::vector<PropertyValueByte> PropertyValueByteString;

typedef std::variant<PropertyValueByte, PropertyValueU32, PropertyValueString> PropertyValueType;

class Property: public Element {
public:
    Property(const std::string &name);

    PropertyType getType() const;
    bool isEmpty() const;

    std::vector<PropertyValueType> getValues() const;
    size_t getValuesCount() const;

    void dump(std::ostream &os, int indent = 0, bool verbose = false) const override;

private:
    void addValue(const PropertyValueType &value);

    static std::string value2String(const PropertyValueType &value);
    static std::string u32ToString(const PropertyValueU32 &value);

    friend class Driver;

private:
    PropertyType m_type;
    std::vector<PropertyValueType> m_values;
};

// Standard Property Subclasses

// class CompatibleProperty : public Property {
// public:
//     CompatibleProperty(const std::string &label, const PropertyValue &value)
//         : Property("compatible", label, { value }) {}

//     CompatibleProperty(const PropertyValue &value)
//         : CompatibleProperty("", value) {}
// };

} // namespace dtparser

#endif // PROPERTY_H