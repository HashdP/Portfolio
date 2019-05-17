#pragma once
#include "Item.hpp"

class Item_Weapon : public Item
{
public:
	Item_Weapon(ItemID id, int num, int maxNum);

	virtual bool Use(PlayerData* playerData) override;
};
