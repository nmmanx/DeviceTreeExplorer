#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <optional>
#include <variant>

template <typename T>
using sp = std::shared_ptr<T>;

template <typename T>
using wp = std::weak_ptr<T>;

namespace dtparser {

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