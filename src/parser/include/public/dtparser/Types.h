#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <variant>

template <typename T>
using sp = std::shared_ptr<T>;

template <typename T>
using wp = std::weak_ptr<T>;

template <typename T>
using up = std::unique_ptr<T>;

namespace dtparser {

class Reference;

using PropertyValueByte = uint8_t;
using PropertyValueU32 = uint32_t;
using PropertyValueReference = sp<Reference>;
using PropertyValueString = std::string;

using PropertyValueU32Array = std::vector<PropertyValueU32>;
using PropertyValueByteString = std::vector<PropertyValueByte>;

using PropertyValueType = std::variant<PropertyValueByte, PropertyValueU32, PropertyValueReference, PropertyValueString>;

struct SourceLocation {
    std::string filename;
    uint32_t beginLine;
    uint32_t beginColumn;
    uint32_t endLine;
    uint32_t endColumn;

    static const SourceLocation NOT_SPECIFIED;
};

} // namespace dtparser

#endif // COMMON_H