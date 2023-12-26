#pragma once
#include "customitem.h"
#include <QAbstractItemModel>

class CustomItemModel : public QAbstractItemModel {
    int m_rowNumber = 0;
    CustomItem* m_rootItem;

public:
    CustomItemModel(QObject* parent = nullptr)
        : QAbstractItemModel(parent) {}

    ~CustomItemModel() { delete m_rootItem; }
    /*visual = 0*/
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override {
        if (row < 0 || column < 0) {
            return QModelIndex();
        }

        if (!hasIndex(row, column, parent))
            return QModelIndex();

        CustomItem* parentItem;

        if (!parent.isValid()) {
            parentItem = m_rootItem;
        } else {
            parentItem = static_cast<CustomItem*>(parent.internalPointer());
        }

        CustomItem* childItem = parentItem->child(row);
        if (childItem)
            return createIndex(row, column, childItem);
        return QModelIndex();
    }

    /*visual = 0*/
    QModelIndex parent(const QModelIndex& child) const override {
        if (!child.isValid())
            return QModelIndex();

        CustomItem* childItem = static_cast<CustomItem*>(child.internalPointer());
        CustomItem* parentItem = childItem->parent();

        if (!parentItem || parentItem == m_rootItem)
            return QModelIndex();

        return createIndex(parentItem->childrenCount(), 0, parentItem);
    }

    //    QModelIndex sibling(int row, int column, const QModelIndex &idx) const {

    //    }

    /*visual = 0*/
    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        int row = parent.row();
        int column = parent.column();

        CustomItem* parentItem = nullptr;

        if (!parent.isValid()) {
            parentItem = m_rootItem;
        } else {
            parentItem = static_cast<CustomItem*>(parent.internalPointer());
        }

        //        qDebug() << row << ',' << column << ":" << parentItem->elementName() << "\t attributeCount: " << parentItem->attributeCount() << "\t childrenCount: " << parentItem->childrenCount();

        return parentItem->childrenCount();
    }

    /*visual = 0*/
    int columnCount(const QModelIndex& parent = QModelIndex()) const override { return 3; }

    bool hasChildren(const QModelIndex& index = QModelIndex()) const override {
        bool retVal = true;

        if (!index.isValid()) { // root
            return true;
        }
        if (CustomItem* item = static_cast<CustomItem*>(index.internalPointer())) {
            return (item->childrenCount()) > 0;
        } else {
            retVal = false;
        }

        return retVal;
    }

    /*visual = 0*/
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid())
            return QVariant();

        int row = index.row();
        int column = index.column();

        CustomItem* item = static_cast<CustomItem*>(index.internalPointer());
        switch (role) {
            case Qt::DisplayRole: {
                simdjson::ondemand::json_type type = item->itemType();
                return item->attribute().at(column);
            } break;
            case Qt::EditRole: {
                return item->attribute().at(column);
            } break;
            default:
                return QVariant();
                break;
        }

        return QVariant();
    }

    Qt::ItemFlags flags(const QModelIndex& index) const override {
        if (!index.isValid()) {
            return Qt::NoItemFlags;
        }

        int col = index.column();
        auto item = static_cast<CustomItem*>(index.internalPointer());

        switch (col) {
        case 0:
            return Qt::ItemIsUserCheckable | QAbstractItemModel::flags(index);
        case 1:
        case 2:
            return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
        default:
            return QAbstractItemModel::flags(index);
        }
    }

    bool setData(const QModelIndex& index, const QVariant& value, int role) {
        int row = index.row();
        int column = index.column();
        CustomItem* item = static_cast<CustomItem*>(index.internalPointer());
        if (Qt::EditRole == role) {
            if (item->itemType() == simdjson::ondemand::json_type::object) {
                if (column == 0) {
                    item->setValue(column, value.toString());
                    emit dataChanged(index, index, {Qt::EditRole});
                    return true;
                }
                if (column == 1) {
                    item->setValue(column, value.toString());
                    emit dataChanged(index, index, {Qt::EditRole});
                    return true;
                }
            }

            if (item->itemType() == simdjson::ondemand::json_type::string) {
                if (column == 0) {
                    item->setValue(column, value.toString());
                    emit dataChanged(index, index, {Qt::EditRole});
                    return true;
                }
                if (column == 1) {
                    return false;
                }
            }
        }

        return false;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Value");
            case 2:
                return tr("XPath");
            default:
                break;
            }
        }
        return QVariant();
    }
    void setRootItem(CustomItem* newRootItem) { m_rootItem = newRootItem; }
    CustomItem* getRootItem() { return m_rootItem; }
};
