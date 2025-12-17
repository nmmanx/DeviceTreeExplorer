#ifndef DEVICE_TREE_MODE_H
#define DEVICE_TREE_MODE_H

#include <QAbstractItemModel>
#include <QUrl>

#include "dtparser/DeviceTreeSource.h"
#include "TreeItem.h"

using namespace dtparser;

class DeviceTreeModel: public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QUrl dtsFilePath READ getDtsFilePath WRITE setDtsFilePath NOTIFY dtsFilePathChanged)
public:
    Q_DISABLE_COPY_MOVE(DeviceTreeModel)

    DeviceTreeModel();
    ~DeviceTreeModel() override;

    QModelIndex index(int row, int column,
                              const QModelIndex &parent) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    QModelIndex sibling(int row, int column, const QModelIndex &idx) const;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QUrl getDtsFilePath() const;
    void setDtsFilePath(const QUrl &dtsFilePath);

signals:
    void dataChanged();
    void dataError(const QString &error);

private:
    TreeItem *m_rootItem;
    DeviceTreeSource *m_dts;
    DeviceTree *m_dt;

    void onDtsFilePathChanged(const QUrl &dtsFilePath);

signals:
    void dtsFilePathChanged(const QUrl &dtsFilePath);
};

#endif