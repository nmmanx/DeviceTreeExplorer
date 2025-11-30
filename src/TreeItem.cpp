#include "TreeItem.h"
#include "dtparser/Property.h"

TreeItem::TreeItem(const sp<Node> &node, TreeItem *parent)
    : m_dtNode(node), m_parent(parent)
{
}

TreeItem::~TreeItem() {
    qDeleteAll(m_children);
}

TreeItem *TreeItem::childAt(int row)
{
    return m_children.at(row);
}

int TreeItem::childCount() const
{
    return m_children.count();
}

int TreeItem::columnCount() const
{
    return static_cast<int>(TreeItemColumn::PROPERTY_0) + m_dtNode->getProperties().size();
}

QVariant TreeItem::data(int column) const
{
    if (column >= columnCount()) {
        return QVariant::Invalid;
    }
    switch (column)
    {
    case static_cast<int>(TreeItemColumn::PATH):
        return QString::fromStdString(m_dtNode->getPath());
    case static_cast<int>(TreeItemColumn::LABEL):
        return QString::fromStdString(m_dtNode->getPrimaryLabel().value_or(Label{}).name);
    case static_cast<int>(TreeItemColumn::NAME):
        return QString::fromStdString(m_dtNode->getName());
    case static_cast<int>(TreeItemColumn::UNIT_ADDRESS):
        return QString::fromStdString(m_dtNode->getUnitAddress());
    default:
        auto prop = m_dtNode->getProperties().at(column - static_cast<int>(TreeItemColumn::PROPERTY_0));
        QString propName = QString::fromStdString(prop->getName());
        QVariantList columnData;

        // first item is the property label
        columnData.append(QString::fromStdString(prop->getPrimaryLabel().value_or(Label{}).name));
        // second item is the property name
        columnData.append(propName);

        // the rest are the string representation of the property values
        for (const auto &value : prop->getValues()) {
            columnData.append(QString::fromStdString(Property::value2String(value)));
        }

        return columnData;
    }
}

int TreeItem::getRow() const
{
    if (m_parent) {
        return m_parent->m_children.indexOf(this);
    }
    return 0;
}

TreeItem *TreeItem::getParent() const
{
    return m_parent;
}

// build the whole subtree started from the given node
void TreeItem::populate()
{
    for (const auto &dtChildNode : m_dtNode->getChildren()) {
        auto child = new TreeItem(dtChildNode, this);
        child->populate();
        m_children.append(child);
    }
}
