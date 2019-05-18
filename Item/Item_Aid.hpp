#pragma once
#include "Item/Item.hpp"

class Item_Aid : public Item
{
private:
	int value;

public:
	Item_Aid(ItemID id, int num, int maxNum, int value);
	virtual ~Item_Aid() = default;

	virtual bool Use(int itemIndex, int weaponIndex, ItemLayer* itemLayer, ObjectLayer* objectLayer) override;
};
