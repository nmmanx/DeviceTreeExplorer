#ifndef PROPERTY_H
#define PROPERTY_H

#include "dtparser/Types.h"
#include "dtparser/Element.h"

namespace dtparser {

class Reference;

class Property: public Element {
public:
    bool isEmpty() const;

    std::vector<PropertyValueType> getValues() const;
    size_t getValuesCount() const;

    std::optional<uint32_t> getAsU32() const;
    std::optional<sp<Reference>> getAsReference() const;
    std::optional<std::string> getAsString() const;
    std::optional<uint8_t> getAsByte() const;

    std::optional<std::vector<std::string>> getAsStringList() const;
    std::optional<std::vector<uint32_t>> getAsU32Array() const;
    std::optional<std::vector<uint8_t>> getAsByteString() const;

    void dump(std::ostream &os, int indent = 0, bool verbose = false) const override;

    static bool isU32(const PropertyValueType &value);
    static bool isReference(const PropertyValueType &value);
    static bool isString(const PropertyValueType &value);
    static bool isByte(const PropertyValueType &value);

    static std::string value2String(const PropertyValueType &value);

private:
    Property(const std::string &name);

    void addValue(const PropertyValueType &value);

    friend class Driver;

private:
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