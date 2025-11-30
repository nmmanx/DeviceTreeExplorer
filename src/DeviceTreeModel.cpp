#include "DeviceTreeModel.h"

DeviceTreeModel::DeviceTreeModel(const sp<DeviceTree> &dt)
    : m_dt(dt)
{
    m_rootItem = new TreeItem(m_dt->getRootNode());
    m_rootItem->populate();
}

DeviceTreeModel::~DeviceTreeModel()
{
    delete m_rootItem;
}

QModelIndex DeviceTreeModel::index(int row, int column,
                            const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeItem *parentItem = parent.isValid() ? 
        static_cast<TreeItem*>(parent.internalPointer()) : m_rootItem;

    if (auto *childItem = parentItem->childAt(row)) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex DeviceTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    auto *childItem = static_cast<TreeItem*>(child.internalPointer());
    TreeItem *parentItem = childItem->getParent();

    return parentItem != m_rootItem
        ? createIndex(parentItem->getRow(), 0, parentItem) : QModelIndex{};
}

QModelIndex DeviceTreeModel::sibling(int row, int column, const QModelIndex &idx) const
{
    return index(row, column, idx.parent());
}

int DeviceTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0; // only the first column has children
    }

    const TreeItem *parentItem = parent.isValid()
        ? static_cast<const TreeItem*>(parent.internalPointer())
        : m_rootItem;

    return parentItem->childCount();
}

int DeviceTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    }
    return m_rootItem->columnCount();
}

QVariant DeviceTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant::Invalid;
    }
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
}
