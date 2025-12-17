#ifndef DEVICE_TREE_VIEW_H
#define DEVICE_TREE_VIEW_H

#include <QQuickPaintedItem>

#include "DeviceTreeModel.h"

struct SubTreeGeometry {
    // Bounding box of the subtree
    QRect rect;
    QModelIndex rootIndex;

    QList<SubTreeGeometry*> children;
    SubTreeGeometry* parent = nullptr;

    // Bounding box of the root node
    // Lazy initialization suring drawing
    QRect rootRect;
};

class DeviceTreeItem: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(DeviceTreeModel* model READ getModel WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(float scale READ getScale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(int nodeWidth READ getNodeWidth WRITE setNodeWidth)
    Q_PROPERTY(int nodeHeight READ getNodeHeight WRITE setNodeHeight)
    Q_PROPERTY(int horizontalSpacing READ getHorizontalSpacing WRITE setHorizontalSpacing)
    Q_PROPERTY(int verticalSpacing READ getVerticalSpacing WRITE setVerticalSpacing)
    QML_ELEMENT
public:
    DeviceTreeItem(QQuickItem *parent = nullptr);
    ~DeviceTreeItem();

    DeviceTreeModel* getModel() const;
    void setModel(DeviceTreeModel* model);

    float getScale() const;
    void setScale(float scale);

    int getNodeWidth() const;
    void setNodeWidth(int nodeWidth);

    int getNodeHeight() const;
    void setNodeHeight(int nodeHeight);

    int getVerticalSpacing() const;
    void setVerticalSpacing(int verticalSpacing);

    int getHorizontalSpacing() const;
    void setHorizontalSpacing(int horizontalSpacing);

    void paint(QPainter *painter) override;

private:
    DeviceTreeModel* m_model = nullptr;
    float m_scale = 1.0f;
    int m_nodeWidth = 200;
    int m_nodeHeight = 50;
    int m_horizontalSpacing = 20;
    int m_verticalSpacing = 10;

    SubTreeGeometry* tree;

    bool m_dataReady = false;

    void initTree();
    void updateGeometry() const;
    void updateGeometryRecursive(QPoint topLeft, SubTreeGeometry *geometry) const;

    void onModelChanged();

signals:
    void modelChanged();
    void scaleChanged();
};

#endif
