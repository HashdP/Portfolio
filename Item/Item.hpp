#pragma once
#include "cocos2d.h"
#include "CocosRef.hpp"

enum ItemID
{
	None,

	Branch,
	IronScrap,
	GrainOfGold,

	GreenBall,
	PurpleBall,
	OrangeBall,
	RedBall,

	Bullet,
	Aid,
	GreenSpray,
	PurpleSpray,
	OrangeSpray,
	RedSpray,

	HandGunParts,
	AssaultRifleParts,

	Weapon_HandGun,
	Weapon_AssaultRifle,
	Weapon_GoldHandGun,
	Weapon_GoldAssaultRifle
};

class ItemLayer;
class ObjectLayer;

class Item
{
private:
	ItemID id;
	int num;
	int maxNum;

	//Itemはしばしばコピーされるため、sharedにしておく
	myutil::cocos_shared_ref<cocos2d::Sprite> sprite;

public:
	Item();
	Item(ItemID id, int num, int maxNum);
	virtual ~Item() = default;

	void SetNum(int num);
	inline ItemID GetID() const { return id; }
	inline int GetNum() const { return num; }
	inline int GetMaxNum() const { return maxNum; }

	virtual bool Use(int itemIndex, int weaponIndex, ItemLayer* itemLayer, ObjectLayer* objectLayer) { return false; }

	cocos2d::Sprite* GetItemImage(bool numLabel = true);
};
