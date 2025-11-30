#ifndef DEVICE_TREE_MODE_H
#define DEVICE_TREE_MODE_H

#include <QAbstractItemModel>

#include "dtparser/DeviceTree.h"
#include "TreeItem.h"

using namespace dtparser;

class DeviceTreeModel: public QAbstractItemModel
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(DeviceTreeModel)

    DeviceTreeModel(const sp<DeviceTree> &dt);
    ~DeviceTreeModel() override;

    QModelIndex index(int row, int column,
                              const QModelIndex &parent) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    QModelIndex sibling(int row, int column, const QModelIndex &idx) const;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

private:
    sp<DeviceTree> m_dt;
    TreeItem *m_rootItem;
};

#endif