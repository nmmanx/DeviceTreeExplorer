#include "DeviceTreeModel.h"

DeviceTreeModel::DeviceTreeModel()
    : m_dts(nullptr), m_rootItem(nullptr)
{
    connect(this, &DeviceTreeModel::dtsFilePathChanged, this, &DeviceTreeModel::onDtsFilePathChanged);
}

DeviceTreeModel::~DeviceTreeModel()
{
    delete m_rootItem;
    delete m_dts;
}

QModelIndex DeviceTreeModel::index(int row, int column,
                            const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    // The root item has no parent
    if (!parent.isValid()) {
        return createIndex(row, column, m_rootItem);
    }

    // Get the parent item
    TreeItem *parentItem = static_cast<TreeItem*>(parent.internalPointer());

    // Create the index for the child
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

    // Root node
    if (parentItem == nullptr) {
        return QModelIndex{};
    }

    return createIndex(parentItem->getRow(), 0, parentItem);
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

    // Count rows of top levels
    if (!parent.isValid()) {
        return 1;
    }
    const TreeItem *parentItem = static_cast<const TreeItem*>(parent.internalPointer());
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

void DeviceTreeModel::setDtsFilePath(const QUrl &dtsFilePath)
{
    emit dtsFilePathChanged(dtsFilePath);
}

QUrl DeviceTreeModel::getDtsFilePath() const
{
    if (m_dts == nullptr) {
        return QUrl();
    }

    auto path = QString::fromStdString(m_dts->getFilePath().string());
    return QUrl::fromLocalFile(path);
}

void DeviceTreeModel::onDtsFilePathChanged(const QUrl &dtsFilePath)
{
    if (m_dts != nullptr) {
        delete m_dts;
    }

    if (dtsFilePath.isEmpty() || !dtsFilePath.isLocalFile()) {
        return;
    }
    qDebug() << "DTS file path changed to:" << dtsFilePath;

    auto path = dtsFilePath.toLocalFile().toStdString();
    m_dts = new DeviceTreeSource(path);

    // parse dts
    auto [ parseResult, dt ] = m_dts->parse();

    if (parseResult.success) {
        m_dt = dt.release();
        
        if (m_rootItem) {
            delete m_rootItem;
        }
        m_rootItem = new TreeItem(m_dt->getRootNode());
        m_rootItem->populate();

        emit dataChanged();
    } else {
        qDebug() << "Failed to parse DTS file: " << parseResult.errorMessage;
        emit dataError(QString::fromStdString(parseResult.errorMessage));
    }
}
