#ifndef DEVICE_TREE_VIEW_H
#define DEVICE_TREE_VIEW_H

#include <QQuickPaintedItem>

#include "DeviceTreeModel.h"

class DeviceTreeItem: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(DeviceTreeModel* model READ getModel WRITE setModel NOTIFY modelChanged)
    QML_ELEMENT
public:
    DeviceTreeItem(QQuickItem *parent = nullptr);
    ~DeviceTreeItem();

    DeviceTreeModel* getModel() const;
    void setModel(DeviceTreeModel* model);

    void paint(QPainter *painter) override;

private:
    DeviceTreeModel* m_model = nullptr;

signals:
    void modelChanged();
};

#endif
