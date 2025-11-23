#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

#include "dtparser/DeviceTreeSource.h"

#include "dtparser.hpp"

#define YY_DECL \
int yylex(yy::parser::semantic_type* const yylval, yy::parser::location_type *loc)

extern YY_DECL;

extern int scan_begin(const char *filename);
extern int scan_end();

namespace dtparser {

struct Element {
    int id;
    std::string name;
    uint32_t line;
    std::string file;
};

typedef std::shared_ptr<const dtparser::Element> ElementRef;

class IDriver {
public:
    virtual ~IDriver() {};

    virtual ElementRef newDirective(const std::string& directive) = 0;
    virtual ElementRef newNode(const std::string &name, const std::string &label) = 0; 
    virtual ElementRef newNode(const std::string &name) = 0;
    virtual ElementRef newProperty(const std::string &name, const std::vector<std::string> &values) = 0;

    virtual void buildHierarchy(const ElementRef &child, const ElementRef &parent) = 0;
    virtual void buildHierarchy(const ElementRef &parent, const std::vector<ElementRef> &chilren) = 0;

    virtual ParseResult parse(const char* dtsFile, DeviceTree *dt) = 0;
};

class Driver: public IDriver
{
public:
    Driver();
    ~Driver() override {};

    ElementRef newDirective(const std::string& directive) override;
    ElementRef newNode(const std::string &name, const std::string &label) override;
    ElementRef newNode(const std::string &name) override;
    ElementRef newProperty(const std::string &name, const std::vector<std::string> &values) override;
    void buildHierarchy(const ElementRef &child, const ElementRef &parent) override;
    void buildHierarchy(const ElementRef &parent, const std::vector<ElementRef> &chilren) override;

    ParseResult parse(const char* dtsFile, DeviceTree *dt) override;
};

} // namespace dtparser

#endif // DRIVER_H