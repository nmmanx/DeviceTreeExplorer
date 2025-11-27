#ifndef PROPERTY_H
#define PROPERTY_H

#include <typeinfo>
#include <variant>

#include "dtparser/Common.h"
#include "dtparser/Element.h"

namespace dtparser {

class Reference;

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
typedef uint32_t PropertyValueU32;
typedef sp<Reference> PropertyValueReference;
typedef std::string PropertyValueString;

typedef std::vector<PropertyValueU32> PropertyValueU32Array;
typedef std::vector<PropertyValueByte> PropertyValueByteString;

typedef std::variant<PropertyValueByte, PropertyValueU32, 
    PropertyValueReference, PropertyValueString> PropertyValueType;

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
    static bool isU32(const PropertyValueType &value);
    static bool isReference(const PropertyValueType &value);
    static bool isString(const PropertyValueType &value);
    static bool isByte(const PropertyValueType &value);

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