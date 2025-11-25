#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <cstdint>
#include <functional>

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

typedef std::function<void(const std::string&, const Label*)> LabelLinker; 

struct Reference {
    std::string name;
    SourceLocation location;
    const Label* label = nullptr;

    LabelLinker linker = [&](const std::string &name, const Label* label) {
        if (this->name == name) {
            this->label = label;
        }
    };
};

class Labelable {
public:
    Labelable() = default;

    void addLabel(const std::string &name, int pos = -1,
        const SourceLocation &loc = SourceLocation::NOT_SPECIFIED)
    {
        if (name.empty()) {
            return;
        }
        Label label = Label {
            name: name,
            location: loc,
            target: this,
            position: pos
        };
        m_labels.push_back(label);
    }

    std::vector<Label> getLabels() const {
        return m_labels;
    }

protected:
    std::vector<Label> m_labels;
};

class Element: public Labelable
{
public:
    Element(const std::string &name)
        : m_name(name), m_parent(nullptr), m_isDeleted(false) {};

    virtual ~Element() {}

    std::string getName() const { return m_name; }

    void setDeleted(bool deleted) { m_isDeleted = deleted; }
    bool isDeleted() const { return m_isDeleted; }

    sp<Element> getParent() const {
        return m_parent;
    }

private:
    void setParent(const sp<Element> &parent) {
        this->m_parent = parent;
    }

protected:
    std::string m_name;
    bool m_isDeleted;
    sp<Element> m_parent;
};

} // namespace dtparser

#endif // ELEMENT_H