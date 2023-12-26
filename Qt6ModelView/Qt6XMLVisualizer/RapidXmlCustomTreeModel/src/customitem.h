#pragma once
#include <QVariant>

class CustomItem {
public:
    enum class ItemType { ELEMENT, ATTRIBUTE };

    CustomItem(CustomItem* parent = nullptr)
        : m_parentItem(parent) {}

    CustomItem* child(int i) {
        CustomItem* m_childItem = m_childrenItems.value(i);
        if (m_childItem) {
            return m_childItem;
        }
        return nullptr;
    }
    ~CustomItem() {
        QHash<int, CustomItem*>::iterator it;
        for (it = m_childrenItems.begin(); it != m_childrenItems.end(); ++it) {
            delete it.value();
        }
        m_childrenItems.clear();
    }

    int childrenCount() { return m_childrenItems.count(); }
    bool hasChildren() { return m_childrenItems.count() > 0; }
    void addChild(CustomItem* child) {
        child->setParent(this);
        int i = m_childrenItems.count();
        m_childrenItems[i] = child;
    }

    void addAttribute(QStringList l) {
        m_attribute = l;
    }
    QStringList attribute() const { return m_attribute; }

    void setValue(int column, QString value) {
        m_attribute[column] = value;
    }

    QString elementName() const { return m_elementName; }
    void setElementName(const QString& newElementName) { m_elementName = newElementName; }

    void setParent(CustomItem* parent) { m_parentItem = parent; }
    CustomItem* parent() { return m_parentItem; }

    bool checked() const { return m_checked; }
    void setChecked(bool newChecked) { m_checked = newChecked; }

    ItemType itemType() const;
    void setItemType(ItemType newItemType);

private:
    QStringList m_attribute{};

    QHash<int, CustomItem*> m_childrenItems{};
    CustomItem* m_parentItem = nullptr;

    bool m_checked = false;
    QString m_elementName;

    ItemType m_itemType;
};
