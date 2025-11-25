#include "dtparser/Element.h"

namespace dtparser {

const SourceLocation SourceLocation::NOT_SPECIFIED = SourceLocation {};

const Label* Labelable::addLabel(const std::string &name, int pos,
    const SourceLocation &loc)
{
    if (name.empty()) {
        return nullptr;
    }
    Label label = Label {
        name: name,
        location: loc,
        target: this,
        position: pos
    };
    m_labels.push_back(label);
    return &m_labels.back();
}

std::optional<Label> Labelable::getPrimaryLabel() const {
    auto it = std::find_if(m_labels.begin(), m_labels.end(), [](const Label &label) {
        return label.position == -1;
    });
    if (it == m_labels.end()) {
        return std::nullopt;
    }
    return *it;
}

std::vector<Label> Labelable::getLabels() const {
    return m_labels;
}

} // namespace dtparser
