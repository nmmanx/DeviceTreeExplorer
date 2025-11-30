#include <QPainter>

#include "DeviceTreeItem.h"

DeviceTreeItem::DeviceTreeItem(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_model(nullptr)
{
    
}

DeviceTreeItem::~DeviceTreeItem()
{
}

DeviceTreeModel* DeviceTreeItem::getModel() const
{
    return m_model;
}

void DeviceTreeItem::setModel(DeviceTreeModel* model)
{
    if (m_model != model) {
        m_model = model;
        emit modelChanged();
    }
}

void DeviceTreeItem::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::black);
    painter->setBrush(Qt::red);

    QRectF rect(0, 0, 200, 50);
    painter->drawRect(rect);
}

