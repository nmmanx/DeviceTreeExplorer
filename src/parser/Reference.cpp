#include <iostream>

#include "dtparser/Reference.h"
#include "dtparser/Node.h"

static std::string getPath(const std::string &name)
{
    auto start = name.find_first_not_of(" {");
    if (start == std::string::npos) {
        return name;
    }

    auto end = name.find_last_not_of(" }");
    if (end == std::string::npos || end < start) {
        return name;
    }

    return name.substr(start, end - start + 1);
}

namespace dtparser
{

Reference::Reference(const std::string &name, const SourceLocation &loc)
    : m_name(name), m_location(loc)
{
    this->m_resolver = [this](const Label* label, const sp<Node> &node) -> bool {
        if (label != nullptr) {
            if (label->name == this->m_name) {
                this->m_target = *label;
                return true;
            }
        } else if (node != nullptr) {
            if (node->getPath() == getPath(this->m_name)) {
                this->m_target = node;
                return true;
            }
        }
        return false;
    };
}

ReferenceResolver Reference::getResolver() const
{
    return m_resolver;
}

std::string Reference::toString() const
{
    return std::string("&") + m_name;
}

std::optional<wp<Node>> Reference::getTargetNode() const
{
    if (std::holds_alternative<wp<Node>>(m_target)) {
        return std::get<wp<Node>>(m_target);
    }
    return std::nullopt;
}

std::optional<Label> Reference::getTargetLabel() const
{
    if (std::holds_alternative<Label>(m_target)) {
        return std::get<Label>(m_target);
    }
    return std::nullopt;
}

} // namespace dtparser