#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <map>
#include <optional>

#include "dtparser/DeviceTreeSource.h"
#include "dtparser/Node.h"
#include "dtparser/Property.h"
#include "dtparser/Reference.h"

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

    virtual void buildHierarchy(uint32_t parent, const std::vector<uint32_t > &chilren) = 0;
    virtual void setTopLevel(uint32_t id) = 0;

    virtual void addLabel(const std::string &name, uint32_t element, 
        const yy::parser::location_type &loc) = 0;

    virtual sp<Reference> makeReference(const std::string &name, const yy::parser::location_type &loc) = 0;

    virtual ParseResult parse(const char* dtsFile, DeviceTree *dt) = 0;

    virtual void setError(const yy::parser::location_type &loc, const std::string &message) = 0;
};

class Driver: public IDriver
{
public:
    Driver(std::ostream &parserOs);
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

    void buildHierarchy(uint32_t parent, const std::vector<uint32_t> &chilren) override;
    void setTopLevel(uint32_t id) override;

    void addLabel(const std::string &name, uint32_t element, 
        const yy::parser::location_type &loc) override;

    sp<Reference> makeReference(const std::string &name, const yy::parser::location_type &loc) override;

    ParseResult parse(const char* dtsFile, DeviceTree *dt) override;

    void setError(const yy::parser::location_type &loc, const std::string &message) override;

    template <typename T>
    class Registry {
    public:
        Registry() = delete;
        Registry(uint32_t firstId, uint32_t lastId): 
            m_nextId(firstId), kFirstId(firstId), kLastId(lastId) {}

        bool accept(uint32_t key) const;

        uint32_t put(T val);
        std::optional<T> get(uint32_t key) const;

        std::vector<T> getAll() const;

        void reset() {
            m_map.clear();
        }

    private:
        uint32_t m_nextId;
        const uint32_t kFirstId;
        const uint32_t kLastId;

        std::map<uint32_t, T> m_map;
    };

private:
    SourceLocation convertLocation(const yy::parser::location_type &loc);
    
    std::string value2String(const PropertyValueTypeOrLabel &valueOrLabel);

    bool isNode(uint32_t id) const;
    bool isProperty(uint32_t id) const;
    bool isDirective(uint32_t id) const;
    bool isPropertyValue(uint32_t id) const;

    void buildPaths(const sp<Node> &root);
    sp<Node> buildTree();
    void resolveReferences(const sp<Node> &root);
    
    std::vector<sp<Node>> flatten(const sp<Node> &root);

    void cleanUp();

private:
    static const uint32_t NODE_ID_FIRST = 1;
    static const uint32_t NODE_ID_LAST = 1 << 24;

    static const uint32_t PROP_ID_FIRST = NODE_ID_LAST + 1;
    static const uint32_t PROP_ID_LAST = PROP_ID_FIRST + (1 << 24);

    static const uint32_t DIRECTIVE_ID_FIRST = PROP_ID_LAST + 1;
    static const uint32_t DIRECTIVE_ID_LAST = DIRECTIVE_ID_FIRST + (1 << 24);

    static const uint32_t PROP_VALUE_ID_FIRST = DIRECTIVE_ID_LAST + 1;
    static const uint32_t PROP_VALUE_ID_LAST = PROP_VALUE_ID_FIRST + (1 << 24);

    Registry<sp<Node>> m_nodes;
    Registry<sp<Property>> m_properties;
    Registry<sp<Directive>> m_directives;
    Registry<PropertyValueTypeOrLabel> m_propertyValues;

    std::vector<ReferenceResolver> m_labelReferenceResolvers;
    std::vector<ReferenceResolver> m_pathReferenceResolvers;
    std::vector<Label> m_labels;
    std::vector<sp<Node>> m_topLevels;

    std::ostream &m_parserOs;

    yy::parser::location_type m_errorLocation;
    std::string m_errorMessage;
};

} // namespace dtparser

#endif // DRIVER_H