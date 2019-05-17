#pragma once
#include "cocos2d.h"
#include "Item.hpp"
#include <array>

class ObjectLayer;
class GUILayer;

class ItemLayer : public cocos2d::Layer
{
private:
	ItemLayer() = default;

	GUILayer* guiLayer;

	std::array<std::unique_ptr<Item>, 24> items;
	std::array<std::unique_ptr<Item>, 3> weapons;

public:
	static ItemLayer* create(GUILayer* guiLayer);
	bool init(GUILayer* guiLayer);


	bool IsExistItem(ItemID itemID, int num);
	bool IsExistItemEmpty(ItemID itemID, int num);

	void SetItem(int index, bool playSound = true);
	void CreateItem(ItemID itemID, int num, int index);
	bool AddItem(ItemID itemID, int num);
	void RemoveItem(int itemIndex);
	bool RemoveItem(ItemID itemID, int num);
	void UseItem(int itemIndex, int weaponIndex, ObjectLayer* objectLayer);
	inline Item GetItem(int index) const { return *items[index]; }
	inline unsigned GetMaxItemNum() const { return items.size(); }

	void SetWeapon(int index, bool playSound = true);
	void AddWeapon(ItemID itemID, int index);
	inline Item GetWeapon(int index) const { return *weapons[index]; }
	inline unsigned GetMaxWeaponNum() const { return weapons.size(); }

	void EquipWeapon(int itemIndex, int weaponIndex);
};
