#include <QPainter>
#include <QStack>

#include "DeviceTreeItem.h"

DeviceTreeItem::DeviceTreeItem(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_model(nullptr), tree(nullptr)
{
    connect(this, &DeviceTreeItem::modelChanged, &DeviceTreeItem::onModelChanged);
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
        if (m_model) {
            disconnect(m_model, &DeviceTreeModel::dataChanged, this, &DeviceTreeItem::initTree);
        }
        m_model = model;
        emit modelChanged();
    }
}

float DeviceTreeItem::getScale() const
{
    return m_scale;
}

void DeviceTreeItem::setScale(float scale)
{
    m_scale = scale;
}

void DeviceTreeItem::setNodeWidth(int nodeWidth)
{
    m_nodeWidth = nodeWidth;
}

void DeviceTreeItem::setNodeHeight(int nodeHeight)
{
    m_nodeHeight = nodeHeight;
}

int DeviceTreeItem::getNodeWidth() const
{
    return m_nodeWidth;
}

int DeviceTreeItem::getNodeHeight() const
{
    return m_nodeHeight;
}

void DeviceTreeItem::setVerticalSpacing(int verticalSpacing)
{
    m_verticalSpacing = verticalSpacing;
}

int DeviceTreeItem::getVerticalSpacing() const
{
    return m_verticalSpacing;
}

void DeviceTreeItem::setHorizontalSpacing(int horizontalSpacing)
{
    m_horizontalSpacing = horizontalSpacing;
}

int DeviceTreeItem::getHorizontalSpacing() const
{
    return m_horizontalSpacing;
}

void DeviceTreeItem::paint(QPainter *painter)
{
    if (!m_dataReady) {
        return;
    }

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::black);
    painter->setBrush(Qt::red);

    QStack<SubTreeGeometry*> stack {};
    stack.push(tree);

    while (!stack.isEmpty()) {
        auto curr = stack.pop();
        auto nameIndex = m_model->index(curr->rootIndex.row(),
            (int)TreeItemColumn::NAME, curr->rootIndex.parent());

        int y = curr->rect.y() + curr->rect.height() / 2 - this->m_nodeHeight / 2;
        
        QRect rect(curr->rect.x(), y, this->m_nodeWidth, this->m_nodeHeight);
        curr->rootRect = rect;

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter, m_model->data(nameIndex, Qt::DisplayRole).toString());

        // Draw the connection line from parent to child
        if (curr->parent) {
            auto startX = curr->parent->rootRect.x() + curr->parent->rootRect.width();
            auto startY = curr->parent->rootRect.y() + curr->parent->rootRect.height() / 2;
            auto endX = curr->rootRect.x();
            auto endY = curr->rootRect.y() + curr->rootRect.height() / 2;
            painter->drawLine(QPoint(startX, startY), QPoint(endX, endY));
        }

        for (auto *child : curr->children) {
            stack.push(child);
        }
    }
}

void DeviceTreeItem::onModelChanged()
{
    m_dataReady = false;
    connect(m_model, &DeviceTreeModel::dataChanged, this, &DeviceTreeItem::initTree);
}

void DeviceTreeItem::initTree() {
    QStack<SubTreeGeometry*> stack {};

    qDebug() << "Model data changed, init tree...";

    if (tree != nullptr) {
        delete tree; 
    }

    tree = new SubTreeGeometry();
    tree->rect.setWidth(this->m_nodeWidth);
    tree->rect.setHeight(this->m_nodeHeight);
    tree->rootIndex = m_model->index(0, 0, QModelIndex());
    tree->parent = nullptr;

    stack.push(tree);

    while (!stack.isEmpty()) {
        auto parent = stack.pop();
        int childCount = m_model->rowCount(parent->rootIndex);

        for (int i = 0; i < childCount; ++i) {
            SubTreeGeometry *child = new SubTreeGeometry();
            child->rect.setWidth(this->m_nodeWidth);
            child->rect.setHeight(this->m_nodeHeight);
            child->rootIndex = m_model->index(i, 0, parent->rootIndex);

            parent->children.append(child);
            child->parent = parent;

            stack.push(child);
        }
    }

    updateGeometry();
    m_dataReady = true;

    // draw tree
    update();
}

void DeviceTreeItem::updateGeometry() const
{
    if (tree == nullptr) {
        return;
    }
    updateGeometryRecursive(QPoint(0, 0), tree);
    qDebug() << "Geometry updated";
}

void DeviceTreeItem::updateGeometryRecursive(QPoint topLeft, SubTreeGeometry *subtree) const
{
    subtree->rect.setX(topLeft.x());
    subtree->rect.setY(topLeft.y());

    int childX = subtree->rect.x() + this->m_nodeWidth + this->m_horizontalSpacing;

    // These values will be updated in the loop
    int childY = subtree->rect.y();
    int width = 0;

    for (int i = 0; i < subtree->children.size(); ++i) {
        updateGeometryRecursive(QPoint(childX, childY), subtree->children[i]);
        childY += subtree->children[i]->rect.height() + this->m_verticalSpacing;

        if (subtree->children[i]->rect.width() > width) {
            width = subtree->children[i]->rect.width();
        }
    }

    int height = childY - subtree->rect.y();

    subtree->rect.setWidth(std::max(width, this->m_nodeWidth));
    subtree->rect.setHeight(std::max(height, this->m_nodeHeight));
}

