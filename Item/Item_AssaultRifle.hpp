#pragma once
#include "Item/WeaponItem.hpp"

class Item_AssaultRifle : public WeaponItem
{
public:
	Item_AssaultRifle(ItemID id, int num, int maxNum, std::string imgFile, std::string dataKey);
	virtual ~Item_AssaultRifle() = default;

	void CreateAttackMotion(Player& player, std::string baseKey, ItemLayer* itemLayer, ObjectLayer* objectLayer);
};
