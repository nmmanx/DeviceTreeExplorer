#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <map>

#include "dtparser/DeviceTreeSource.h"
#include "dtparser/Node.h"
#include "dtparser/Property.h"

#include "dtparser.hpp"

#define YY_DECL \
int yylex(yy::parser::semantic_type* const yylval, yy::parser::location_type *loc)

extern YY_DECL;

extern int scan_begin(const char *filename);
extern int scan_end();

template <typename T>
using sp = std::shared_ptr<T>;

namespace dtparser {

struct ValueLabel {
    std::string name;
    yy::parser::location_type loc;
};

typedef std::variant<PropertyValueType,ValueLabel> PropertyValueTypeOrLabel;

struct Directive {
    std::string name;
    std::vector<std::string> args;
    SourceLocation location;
};

class IDriver {
public:
    virtual ~IDriver() {};

    virtual uint32_t newDirective(
        const std::string& directive, 
        const std::vector<std::string> &args,
        const yy::parser::location_type &loc) = 0;

    virtual uint32_t newNode(
        const std::string &fullName,
        const yy::parser::location_type &loc) = 0;

    virtual uint32_t newProperty(
        const std::string &name,
        const yy::parser::location_type &loc) = 0;

    virtual uint32_t newPropertyValue(
        const PropertyValueTypeOrLabel &value,
        const yy::parser::location_type &loc) = 0;

    virtual void buildHierarchy(uint32_t child, uint32_t parent) = 0;
    virtual void buildHierarchy(uint32_t parent, const std::vector<uint32_t > &chilren) = 0;

    virtual void addLabel(const std::string &label, uint32_t element, 
        const yy::parser::location_type &loc) = 0;

    virtual Reference makeReference(const std::string &name, const yy::parser::location_type &loc) = 0;

    virtual ParseResult parse(const char* dtsFile, DeviceTree *dt) = 0;
};

class Driver: public IDriver
{
public:
    Driver();
    ~Driver() override {};

    uint32_t newDirective(
        const std::string& directive, 
        const std::vector<std::string> &args,
        const yy::parser::location_type &loc) override;

    uint32_t newNode(
        const std::string &fullName, 
        const yy::parser::location_type &loc) override;

    uint32_t newProperty(
        const std::string &name,
        const yy::parser::location_type &loc) override;

    uint32_t newPropertyValue(
        const PropertyValueTypeOrLabel &value,
        const yy::parser::location_type &loc) override;

    void buildHierarchy(uint32_t child, uint32_t parent) override;
    void buildHierarchy(uint32_t parent, const std::vector<uint32_t > &chilren) override;

    void addLabel(const std::string &label, uint32_t element, 
        const yy::parser::location_type &loc) override;

    Reference makeReference(const std::string &name, const yy::parser::location_type &loc) {
        Reference ref = {
            name: name,
            location: convertLocation(loc)
        };
        // save linker!
        return ref;
    }

    ParseResult parse(const char* dtsFile, DeviceTree *dt) override;

private:
    SourceLocation convertLocation(const yy::parser::location_type &loc);

private:
    static const uint32_t NODE_ID_FIRST = 1;
    static const uint32_t NODE_ID_LAST = 1 << 16;

    static const uint32_t PROP_ID_FIRST = NODE_ID_LAST + 1;
    static const uint32_t PROP_ID_LAST = PROP_ID_FIRST + (1 << 16);

    static const uint32_t DIRECTIVE_ID_FIRST = PROP_ID_LAST + 1;
    static const uint32_t DIRECTIVE_ID_LAST = DIRECTIVE_ID_FIRST + (1 << 16);

    static const uint32_t PROP_VALUE_ID_FIRST = DIRECTIVE_ID_LAST + 1;

    uint32_t m_nextNodeId = NODE_ID_FIRST;
    uint32_t m_nextPropId = PROP_ID_FIRST;
    uint32_t m_nextDirectiveId = DIRECTIVE_ID_FIRST;
    uint32_t m_nextPropValueId = PROP_VALUE_ID_FIRST;

    std::map<uint32_t, sp<Node>> m_nodes;
    std::map<uint32_t, sp<Property>> m_properties;
    std::map<uint32_t, sp<Directive>> m_directives;
    std::map<uint32_t, PropertyValueType> m_propertyValues;
};

} // namespace dtparser

#endif // DRIVER_H