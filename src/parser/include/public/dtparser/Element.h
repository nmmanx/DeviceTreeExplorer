#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <cstdint>
#include <functional>
#include <optional>

#include "dtparser/Common.h"

namespace dtparser {

struct SourceLocation {
    std::string filename;
    uint32_t beginLine;
    uint32_t beginColumn;
    uint32_t endLine;
    uint32_t endColumn;

    static const SourceLocation NOT_SPECIFIED;
};

class Labelable;

struct Label {
    std::string name;
    SourceLocation location;
    Labelable *target;
    int position; // -1 means top-level label. Ex: label: somenode {}
};

typedef std::function<bool(const Label*)> LabelLinker; 

struct Reference {
    std::string name;
    SourceLocation location;
    const Label* label = nullptr;

    LabelLinker linker = [&](const Label* label) -> bool {
        if (label->name == name) {
            this->label = label;
            return true;
        }
        return false;
    };

    std::string toString() const {
        return std::string("&") + name;
    }
};

class Labelable {
public:
    Labelable() = default;

    const Label* addLabel(const std::string &name, int pos = -1,
        const SourceLocation &loc = SourceLocation::NOT_SPECIFIED);

    std::vector<Label> getLabels() const;

    std::optional<Label> getPrimaryLabel() const;

protected:
    std::vector<Label> m_labels;
};

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