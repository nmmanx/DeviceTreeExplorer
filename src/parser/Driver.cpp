#include <iostream>
#include <stdexcept>

#include "Driver.h"

namespace dtparser {

Driver::Driver(std::ostream &parserOs)
    : m_nodes(NODE_ID_FIRST, NODE_ID_LAST),
      m_directives(DIRECTIVE_ID_FIRST, DIRECTIVE_ID_LAST),
      m_properties(PROP_ID_FIRST, PROP_ID_LAST),
      m_propertyValues(PROP_VALUE_ID_FIRST, PROP_VALUE_ID_LAST),
      m_parserOs(parserOs),
      m_rootNode(nullptr) {

}

uint32_t Driver::newDirective(
    const std::string& name, 
    const std::vector<std::string> &args,
    const yy::parser::location_type &loc)
{
    sp<Directive> directive(new Directive {
        name: name, 
        args: args,
        location: convertLocation(loc)
    });
    std::cout << "New Directive: " << name << std::endl;
    return m_directives.put(directive);
}

uint32_t Driver::newNode(
    const std::string &fullName,
    const yy::parser::location_type &loc)
{
    sp<Node> node(new Node(fullName));
    node->setLocation(convertLocation(loc));
    if (fullName == "/") {
        m_rootNode = node;
    }
    std::cout << "New Node: " << fullName << std::endl;
    return m_nodes.put(node);
}

uint32_t Driver::newProperty(
    const std::string &name, 
    const yy::parser::location_type &loc)
{
    sp<Property> property(new Property(name));
    property->setLocation(convertLocation(loc));
    std::cout << "New Property: " << name << std::endl;
    return m_properties.put(property);
}

uint32_t Driver::newPropertyValue(
        const PropertyValueTypeOrLabel &value,
        const yy::parser::location_type &loc)
{
    std::cout << "New Property Value: " << value2String(value) << std::endl;
    return m_propertyValues.put(value);
}

void Driver::buildHierarchy(uint32_t parent, const std::vector<uint32_t> &chilren)
{
    if (chilren.empty()) {
        return;
    }

    if (isNode(parent)) {
        sp<Node> node = m_nodes.get(parent).value();

        for (uint32_t child : chilren) {
            if (isNode(child)) {
                sp<Node> childNode = m_nodes.get(child).value();
                node->addChild(childNode);
                childNode->setParent(node);
                std::cout << "Node<-Node: " << node->getName() << "<-" << childNode->getName() << std::endl;
            } else if (isProperty(child)) {
                sp<Property> childProperty = m_properties.get(child).value();
                node->addProperty(childProperty);
                childProperty->setParent(node);
                std::cout << "Node<-Prop: " << node->getName() << "<-" << childProperty->getName() << std::endl;
            }
        }
    } else if (isProperty(parent)) {
        sp<Property> prop = m_properties.get(parent).value();

        for (uint32_t child : chilren) {
            if (isPropertyValue(child)) {
                PropertyValueTypeOrLabel valueOrLabel = m_propertyValues.get(child).value();
                
                if (std::holds_alternative<PropertyValueType>(valueOrLabel)) {
                    PropertyValueType value = std::get<PropertyValueType>(valueOrLabel);
                    std::cout << "Prop<-Value: " << prop->getName() << "<-" << Property::value2String(value) << std::endl;
                    prop->addValue(value);
                } else if (std::holds_alternative<ValueLabel>(valueOrLabel)) {
                    ValueLabel valueLabel = std::get<ValueLabel>(valueOrLabel);
                    Label label {
                        name: valueLabel.name,
                        location: convertLocation(valueLabel.loc),
                        target: prop,
                        position: (int)prop->getValuesCount()
                    };
                    prop->addLabel(label);

                    std::cout << "Prop<-Label: " << prop->getName() << "<-" << label.name << std::endl;
                    m_labels.push_back(label);
                } else {
                    std::cout << "Unknown value type: " << valueOrLabel.index() << std::endl;
                }
            }
        }
    }
}

void Driver::addLabel(const std::string &name, uint32_t element, 
    const yy::parser::location_type &loc)
{
    if (name.empty()) {
        return;
    }
    if (isNode(element)) {
        sp<Node> node = m_nodes.get(element).value();
        Label label {
            name: name,
            location: convertLocation(loc),
            target: node,
            position: -1
        };
        node->addLabel(label);
        m_labels.push_back(label);

    } else if (isProperty(element)) {
        sp<Property> prop = m_properties.get(element).value();
        Label label {
            name: name,
            location: convertLocation(loc),
            target: prop,
            position: -1
        };
        prop->addLabel(label);
        m_labels.push_back(label);
    }
}

sp<Reference> Driver::makeReference(const std::string &name, const yy::parser::location_type &loc)
{
    sp<Reference> ref(new Reference(name, convertLocation(loc)));

    if (name.substr(0, 1) == "{") {
        m_pathReferenceResolvers.push_back(ref->getResolver());
    } else {
        m_labelReferenceResolvers.push_back(ref->getResolver());
    }
    return ref;
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
    parser.set_debug_stream(m_parserOs);

    if (parser.parse() != 0) {
        std::cerr << "Parsing failed for file: " << dtsFile << std::endl;
        result.errorLocation = convertLocation(m_errorLocation);
        result.errorMessage = m_errorMessage;
    }

    scan_end();

    if (m_rootNode != nullptr) {
        dt->m_rootNode = m_rootNode;
        buildPaths(m_rootNode);
        resolveReferences();
        result.success = true;
    } else {
        result.success = false;
        result.errorMessage = "No root node found";
    }
    
    cleanUp();

    return result;
}

void Driver::setError(const yy::parser::location_type &loc, const std::string &message  )
{
    m_errorLocation = loc;
    m_errorMessage = message;
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

std::string Driver::value2String(const PropertyValueTypeOrLabel &valueOrLabel)
{
    return std::visit([](const auto &v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, ValueLabel>) {
            return v.name + ":";
        } else {
            return Property::value2String(v);
        }
    }, valueOrLabel);
}

bool Driver::isNode(uint32_t id) const
{
    return m_nodes.accept(id);
}

bool Driver::isProperty(uint32_t id) const
{
    return m_properties.accept(id);
}

bool Driver::isDirective(uint32_t id) const
{
    return m_directives.accept(id);
}

bool Driver::isPropertyValue(uint32_t id) const
{
    return m_propertyValues.accept(id);
}

void Driver::resolveReferences()
{
    int count = 0;
    for (auto label : m_labels) {
        for (auto resolver : m_labelReferenceResolvers) {
            if (resolver(&label, nullptr)) {
                std::cout << "Resolved reference: " << label.name << std::endl;
                count++;
            }
        }
    }

    auto allNodes = m_nodes.getAll();
    for (auto node : allNodes) {
        for (auto resolver : m_pathReferenceResolvers) {
            if (resolver(nullptr, node)) {
                std::cout << "Resolved reference: " << node->getName() << std::endl;
                count++;
            }
        }
    }

    std::cout << "Resolved " << count << " references" << std::endl;
}

void Driver::buildPaths(const sp<Node> &root)
{
    root->setPath("");
    std::function<void(const sp<Node>&)> pathSetter = [&pathSetter](const sp<Node> &parent) {
        for (const auto &child : parent->getChildren()) {
            child->setPath(parent->getPath() + "/" + child->getName());
            pathSetter(child);
        }
    };
    pathSetter(root);
    root->setPath("/");
}

void Driver::cleanUp()
{
    m_nodes.reset();
    m_properties.reset();
    m_directives.reset();
    m_propertyValues.reset();
    m_labels.clear();
    m_labelReferenceResolvers.clear();
    m_pathReferenceResolvers.clear();
}

template <typename T>
bool Driver::Registry<T>::accept(uint32_t key) const
{
    return key >= kFirstId && key <= kLastId;
}

template <typename T>
uint32_t Driver::Registry<T>::put(T val)
{
    size_t patience = kLastId - kFirstId + 1;

    for (size_t i = 0; i < patience; ++i) {
        auto item = m_map.find(m_nextId);
        if (item == m_map.end()) {
            m_map[m_nextId] = val;
            return m_nextId++;
        }
        m_nextId = (m_nextId == kLastId) ? kFirstId : m_nextId + 1;
    }

    throw std::out_of_range("Registry is full");
}

template <typename T>
std::optional<T> Driver::Registry<T>::get(uint32_t key) const
{
    if (key < kFirstId || key > kLastId) {
        throw std::invalid_argument("Invalid key");
    }
    auto itr = m_map.find(key);
    if (itr == m_map.end()) {
        return std::nullopt;
    }
    return itr->second;
}

template <typename T>
std::vector<T> Driver::Registry<T>::getAll() const
{
    std::vector<T> result;
    for (const auto &item : m_map) {
        result.push_back(item.second);
    }
    return result;
}

} // namespace dtparser
