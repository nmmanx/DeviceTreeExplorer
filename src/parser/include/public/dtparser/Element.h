#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <cstdint>
#include <functional>
#include <optional>

#include "dtparser/Common.h"
#include "dtparser/Reference.h"

namespace dtparser {

class Element: public Labelable
{
public:
    Element(const std::string &name)
        : m_name(name), m_parent(nullptr), m_isDeleted(false) {};

    virtual ~Element() {}
    virtual void dump(std::ostream &os, int indent = 0, bool verbose = false) const = 0;

    std::string getName() const { return m_name; }

    void setDeleted(bool deleted) { m_isDeleted = deleted; }
    bool isDeleted() const { return m_isDeleted; }

    sp<Element> getParent() const {
        return m_parent;
    }

    SourceLocation getLocation() const {
        return m_location;
    }

private:
    void setParent(const sp<Element> &parent) {
        this->m_parent = parent;
    }

    void setLocation(const SourceLocation &loc) {
        m_location = loc;
    }

    friend class Driver;

protected:
    std::string m_name;
    bool m_isDeleted;
    sp<Element> m_parent;
    SourceLocation m_location;
};

} // namespace dtparser

#endif // ELEMENT_H