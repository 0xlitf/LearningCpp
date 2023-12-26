#include "customitem.h"


QJsonValue::Type CustomItem::itemType() const
{
    return m_itemType;
}

void CustomItem::setType(const QJsonValue::Type& type)
{
    m_itemType = type;
}

QHash<int, CustomItem*> CustomItem::childrenItems() const
{
    return m_childrenItems;
}

void CustomItem::setChildrenItems(const QHash<int, CustomItem*>& newChildrenItems)
{
    m_childrenItems = newChildrenItems;
}
