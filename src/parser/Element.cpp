#include "dtparser/Element.h"

namespace dtparser {

const SourceLocation SourceLocation::NOT_SPECIFIED = SourceLocation {};

void Labelable::addLabel(const Label &label) {
    m_labels.push_back(label);
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
