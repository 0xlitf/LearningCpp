#include "customitem.h"


CustomItem::ItemType CustomItem::itemType() const
{
    return m_itemType;
}

void CustomItem::setItemType(CustomItem::ItemType newItemType)
{
    m_itemType = newItemType;
}
