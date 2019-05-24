#pragma once
#include "WeaponItem.hpp"

class Item_HandGun : public WeaponItem
{
public:
	Item_HandGun(ItemID id, int num, int maxNum, std::string imgFile, std::string dataKey);
	virtual ~Item_HandGun() = default;

	void CreateAttackMotion(Player& player, std::string baseKey, ItemLayer* itemLayer, ObjectLayer* objectLayer);
};
