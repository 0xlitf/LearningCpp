
#include "dommodel.h"
#include "domitem.h"
#include <QtXml>

DomModel::DomModel(const QDomDocument &document, QObject *parent)
    : QAbstractItemModel(parent)
    , m_domDocument(document){}

DomModel::~DomModel() {
    delete m_rootItem;
}

int DomModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 3;
}

QVariant DomModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    DomItem *item = static_cast<DomItem *>(index.internalPointer());
    const QDomNode node = item->node();

    if (role == Qt::CheckStateRole && (index.column() == 0) && hasChildren(index)) {
        return static_cast<int>(item->isChecked() ? Qt::Checked : Qt::Unchecked);
    }
    if (role == Qt::FontRole && item->isChecked() && hasChildren(index)) {
        QFont font;
        font.setBold(true);
        return font;
    }
    if (role == Qt::BackgroundRole && item->isChecked() && hasChildren(index)) {
        return QColor(0, 255, 127);
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    switch (index.column()) {
    case 0:
        return node.nodeName();
    case 1: {
        const QDomNamedNodeMap attributeMap = node.attributes();
        QStringList attributes;
        for (int i = 0; i < attributeMap.count(); ++i) {
            QDomNode attribute = attributeMap.item(i);
            attributes << attribute.nodeName() + "=\"" + attribute.nodeValue() + '"';
        }
        return attributes.join(' ');
    }
    case 2:
        return node.nodeValue().split('\n').join(' ');
    default:
        break;
    }

    return item->data(index.column());
}

Qt::ItemFlags DomModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (index.column() == 0) {
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

QVariant DomModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("node.nodeName\t \t \t");
        case 1:
            return tr("attributes\t\t");
        case 2:
            return tr("node.nodeValue\t");
        default:
            break;
        }
    }
    return QVariant();
}

QModelIndex DomModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<DomItem *>(parent.internalPointer());

    DomItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex DomModel::parent(const QModelIndex &child) const {
    if (!child.isValid())
        return QModelIndex();

    DomItem *childItem = static_cast<DomItem *>(child.internalPointer());
    DomItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == m_rootItem)
        return QModelIndex();
    // createIndex(parentItem->row(), 0, parentItem);
    return createIndex(parentItem->row(), 0, parentItem);
}

int DomModel::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0)
        return 0;

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<DomItem *>(parent.internalPointer());

    return parentItem->node().childNodes().count();
}

bool DomModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    DomItem *item = static_cast<DomItem *>(index.internalPointer());

    if (index.column() == 0) {
        if (role == Qt::EditRole) {
            return false;
        }
        if (role == Qt::CheckStateRole) {
            item->setChecked(value.toBool());
            emit dataChanged(index, index);
            return true;
        }
    }
    return QAbstractItemModel::setData(index, value, role);
}
