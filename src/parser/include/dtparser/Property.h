#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <vector>
#include <cstdint>
#include <typeinfo>

#include "dtparser/Node.h"

namespace dtparser {

class Node;

enum class PropertyType: uint8_t {
    EMPTY,
    U32,
    U64,
    PHANDLE,
    STRING,
    STRING_LIST,
    PROP_ENCODED_ARRAY,
};

class Property {
public:
    Property(
        const std::string& name,
        const std::string& value,
        PropertyType type
    ) {}

    PropertyType getType() const {
        return m_type;
    }

    bool isEmpty() const {
        return m_type == PropertyType::EMPTY;
    }

    bool getValueAsU32(uint32_t& outValue) const;
    bool getValueAsU64(uint64_t& outValue) const;
    bool getValueAsString(std::string& outValue) const;
    bool getValueAsStringList(std::vector<std::string>& outValue) const;
    bool getValueAsPhandle(Node*& outValue) const;

private:
    std::string m_name;
    std::string m_value;
    PropertyType m_type;
};

// Standard Property Subclasses

class CompatibleProperty : public Property {
public:
    CompatibleProperty(const std::string& value)
        : Property("compatible", value, PropertyType::STRING_LIST) {}
};

class ModelProperty : public Property {
public:
    ModelProperty(const std::string& value)
        : Property("model", value, PropertyType::STRING) {}
};

class PhandleProperty : public Property {
public:
    PhandleProperty(const std::string& value)
        : Property("phandle", value, PropertyType::PHANDLE) {}
};

class StatusProperty : public Property {
public:
    StatusProperty(const std::string& value)
        : Property("status", value, PropertyType::STRING) {}
};

class AddressCellsProperty : public Property {
public:
    AddressCellsProperty(const std::string& value)
        : Property("#address-cells", value, PropertyType::U32) {}
};

class SizeCellsProperty : public Property {
public:
    SizeCellsProperty(const std::string& value)
        : Property("#size-cells", value, PropertyType::U32) {}
}; 

class RegProperty : public Property {
public:
    RegProperty(const std::string& value)
        : Property("reg", value, PropertyType::PROP_ENCODED_ARRAY) {}
};

class VirtualRegProperty : public Property {
public:
    VirtualRegProperty(const std::string& value)
        : Property("virtual-reg", value, PropertyType::U32) {}
};

class RangesProperty : public Property {
public:
    RangesProperty(const std::string& value)
        : Property("ranges", value, PropertyType::PROP_ENCODED_ARRAY) {}
}; 

class DmaRangesProperty : public Property {
public:
    DmaRangesProperty(const std::string& value)
        : Property("dma-ranges", value, PropertyType::PROP_ENCODED_ARRAY) {}
};

class DmaCoherentProperty : public Property {
public:
    DmaCoherentProperty(const std::string& value)
        : Property("dma-coherent", value, PropertyType::EMPTY) {}
};

class DmaNonCoherentProperty : public Property {
public:
    DmaNonCoherentProperty(const std::string& value)
        : Property("dma-noncoherent", value, PropertyType::EMPTY) {}
};

class NameProperty : public Property {
public:
    NameProperty(const std::string& value)
        : Property("name", value, PropertyType::STRING) {}
};

class DeviceTypeProperty : public Property {
public:
    DeviceTypeProperty(const std::string& value)
        : Property("device_type", value, PropertyType::STRING) {}
};  

} // namespace dtparser

#endif // PROPERTY_H