#include "customitem.h"

rapidjson::Type CustomItem::itemType() const
{
    return m_itemType;
}

void CustomItem::setType(const rapidjson::Type& type)
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

CustomItem::NumberType CustomItem::numberType() const
{
    return m_numberType;
}

void CustomItem::setNumberType(NumberType newNumberType)
{
    m_numberType = newNumberType;
}
