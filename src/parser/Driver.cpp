#include <iostream>

#include "Driver.h"

namespace dtparser {

Driver::Driver() {
    std::cout << "Driver initialized." << std::endl;
}

ElementRef Driver::newDirective(const std::string& directive)
{
    std::cout << "newDirective: " << directive << std::endl;
    return nullptr;
}

ElementRef Driver::newNode(const std::string &name, const std::string &label)
{
    std::cout << "newNode: " << name << " (label: " << label << ")" << std::endl;
    return nullptr;
}

ElementRef Driver::newNode(const std::string &name)
{
    std::cout << "newNode: " << name << std::endl;
    return nullptr;
}

ElementRef Driver::newProperty(const std::string &name, const std::vector<std::string> &values)
{
    std::cout << "newProperty: " << name << " with " << values.size() << " values" << std::endl;
    return nullptr;
}

void Driver::buildHierarchy(const ElementRef &child, const ElementRef &parent)
{
}

void Driver::buildHierarchy(const ElementRef &parent, const std::vector<ElementRef> &chilren)
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

} // namespace dtparser
