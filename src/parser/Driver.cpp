#include <iostream>

#include "Driver.h"

namespace dtparser {

Driver::Driver() {
    std::cout << "Driver initialized." << std::endl;
}

uint32_t Driver::newDirective(const std::string& directive, 
    const std::vector<std::string> &args,
    const yy::parser::location_type &loc)
{
    std::cout << "newDirective: " << directive << std::endl;
    return 0;
}

uint32_t Driver::newNode(const std::string &name, const std::string &label,
    const yy::parser::location_type &loc)
{
    std::cout << "newNode: " << name << " (label: " << label << ")" << std::endl;
    return 0;
}

uint32_t Driver::newProperty(const std::string &name, const std::string &label, const std::vector<PropertyValue> &values,
    const yy::parser::location_type &loc)
{
    std::cout << "newProperty: " << name << " with " << values.size() << " values" << std::endl;
    return 0;
}

void Driver::buildHierarchy(uint32_t child, uint32_t parent)
{
}

void Driver::buildHierarchy(uint32_t parent, const std::vector<uint32_t> &chilren)
{
}

ParseResult Driver::parse(const char* dtsFile, DeviceTree *dt)
{
    ParseResult result;
    result.success = false;

    std::cout << "Parsing file: " << dtsFile << std::endl;

    if (scan_begin(dtsFile) != 0) {
        std::cerr << "Failed to open file: " << dtsFile << std::endl;
        return result;
    }

    yy::parser parser(this);
    parser.set_debug_level(1);

    if (parser.parse() == 0) {
        result.success = true;
    } else {
        std::cerr << "Parsing failed for file: " << dtsFile << std::endl;
    }

    scan_end();
    return result;
}

SourceLocation Driver::convertLocation(const yy::parser::location_type &loc)
{
    SourceLocation targetLoc;
    if (loc.begin.filename) {
        targetLoc.filename = *(loc.begin.filename);
    } else {
        targetLoc.filename = "";
    }
    targetLoc.beginLine = static_cast<uint32_t>(loc.begin.line);
    targetLoc.beginColumn = static_cast<uint32_t>(loc.begin.column);
    targetLoc.endLine = static_cast<uint32_t>(loc.end.line);
    targetLoc.endColumn = static_cast<uint32_t>(loc.end.column);
    return targetLoc;
}

} // namespace dtparser
