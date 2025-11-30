#ifndef TREE_ITEM_H
#define TREE_ITEM_H

#include <QList>
#include <QVariant>

#include "dtparser/Node.h"

using namespace dtparser;

enum class TreeItemColumn: int {
    PATH = 0,
    LABEL,
    NAME,
    UNIT_ADDRESS,
    PROPERTY_0
};

class TreeItem
{
public:
    TreeItem(const sp<Node> &node, TreeItem *parent = nullptr);
    ~TreeItem();

    TreeItem(const TreeItem &other) = delete;
    TreeItem &operator=(const TreeItem &other) = delete;

    TreeItem *childAt(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int getRow() const;
    TreeItem *getParent() const;

    // build the whole subtree started from the given node
    void populate();

private:
    sp<Node> m_dtNode;
    QList<TreeItem*> m_children;
    TreeItem *m_parent;
};

#endif