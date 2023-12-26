#include "customitem.h"

simdjson::ondemand::json_type CustomItem::itemType() const
{
    return m_itemType;
}

void CustomItem::setType(const simdjson::ondemand::json_type& type)
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

simdjson::fallback::number_type CustomItem::numberType() const
{
    return m_numberType;
}

void CustomItem::setNumberType(simdjson::fallback::number_type newNumberType)
{
    m_numberType = newNumberType;
}
