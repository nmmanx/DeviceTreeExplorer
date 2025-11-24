#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <cstdint>

namespace dtparser {

struct SourceLocation {
    std::string filename;
    uint32_t beginLine;
    uint32_t beginColumn;
    uint32_t endLine;
    uint32_t endColumn;
};

class Element
{
public:
    Element(const std::string &name, const std::string &label = "")
        : m_name(name), m_label(label), m_isDeleted(false) {};

    virtual ~Element() {}

    std::string getName() const { return m_name; }
    std::string getLabel() const { return m_label; }

    void setLocation(const SourceLocation &loc) { m_location = loc; }
    SourceLocation getLocation() const { return m_location; }

    void setDeleted(bool deleted) { m_isDeleted = deleted; }
    bool isDeleted() const { return m_isDeleted; }

protected:
    std::string m_name;
    std::string m_label;
    SourceLocation m_location;
    bool m_isDeleted;
};

} // namespace dtparser

#endif // ELEMENT_H