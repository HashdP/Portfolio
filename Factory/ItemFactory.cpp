#include "ItemFactory.hpp"
#include "Item_Aid.hpp"
#include "Item_Spray.hpp"
#include "Item_HandGun.hpp"
#include "Item_AssaultRifle.hpp"
#include "DropItem.hpp"
#include "ObjectLayer.hpp"
#include "Floor.hpp"
#include "Route.hpp"
#include "Area.hpp"

USING_NS_CC;

ItemFactory* ItemFactory::instance = nullptr;

ItemFactory* ItemFactory::getInstance()
{
	if (instance == nullptr)
		instance = new ItemFactory();

	return instance;
}

std::unique_ptr<Item> ItemFactory::CreateItem(ItemID itemID, int num)
{
	Item* item = nullptr;
	
	switch (itemID)
	{
	case ItemID::Weapon_HandGun:          item = new Item_HandGun(itemID, num, 1, "Images/Weapon/wepon_handgun.png",     "Player_HandGun");   break;
	case ItemID::Weapon_GoldHandGun:      item = new Item_HandGun(itemID, num, 1, "Images/Weapon/wepon_goldhandgun.png", "Player_G_HandGun"); break;

	case ItemID::Weapon_AssaultRifle:     item = new Item_AssaultRifle(itemID, num, 1, "Images/Weapon/weapon_assaultrifle.png",     "Player_Assault"); break;
	case ItemID::Weapon_GoldAssaultRifle: item = new Item_AssaultRifle(itemID, num, 1, "Images/Weapon/weapon_goldassaultrifle.png", "Player_G_Assault"); break;

	case ItemID::Aid:                     item = new Item_Aid(itemID, num, 1, 10); break;

	case ItemID::GreenSpray:              item = new Item_Spray(itemID, num, 1, RouteState::Dirt); break;
	case ItemID::PurpleSpray:             item = new Item_Spray(itemID, num, 1, RouteState::HeavyDirt); break;
	case ItemID::OrangeSpray:             item = new Item_Spray(itemID, num, 1, RouteState::BadDirt); break;
	case ItemID::RedSpray:                item = new Item_Spray(itemID, num, 1, RouteState::DangerousDirt); break;

	default:                              item = new Item(itemID, num, 99); break;
	}

	return std::unique_ptr<Item>(item);
}

DropItem* ItemFactory::CreateDropItem(ObjectLayer* objectLayer, const cocos2d::Vec2& position, ItemID itemID, int num)
{
	DropItem* dropItem = nullptr;

	if (0 < num)
	{
		dropItem = DropItem::create(objectLayer, itemLayer, itemID, num);

		//同じブロックにドロップさせる
		dropItem->setPosition(position);

		objectLayer->AddFieldObject(dropItem);
	}

	return dropItem;
}

DropItem* ItemFactory::CreateDropItemOnFloor(ObjectLayer* objectLayer, const cocos2d::Vec2& position, ItemID itemID, int num)
{
	return CreateDropItem(objectLayer, objectLayer->GetArea().GetFloor(position)->getPosition() + Vec2(random<float>(-10.0f, 10.0f), random<float>(-10.0f, 10.0f)), itemID, num);
}

Texture2D* ItemFactory::GetItemTexture(ItemID itemID)
{
	std::string path = "";

	switch (itemID)
	{
	case ItemID::Branch:              path = "Images/Items/item_branch.png"; break;
	case ItemID::IronScrap:           path = "Images/Items/item_ironscrap.png"; break;
	case ItemID::GrainOfGold:         path = "Images/Items/item_grainofgold.png"; break;
	case ItemID::GreenBall:           path = "Images/Items/item_greenball.png"; break;
	case ItemID::PurpleBall:          path = "Images/Items/item_purpleball.png"; break;
	case ItemID::OrangeBall:          path = "Images/Items/item_orangeball.png"; break;
	case ItemID::RedBall:             path = "Images/Items/item_redball.png"; break;
	case ItemID::Bullet:              path = "Images/Items/item_bullet.png"; break;
	case ItemID::HandGunParts:        path = "Images/Items/item_handgunparts.png"; break;
	case ItemID::AssaultRifleParts:   path = "Images/Items/item_assaultrifleparts.png"; break;
	case ItemID::Weapon_HandGun:      path = "Images/Items/item_handgun.png"; break;
	case ItemID::Weapon_AssaultRifle: path = "Images/Items/item_assaultrifle.png"; break;
	case ItemID::Weapon_GoldHandGun:      path = "Images/Items/item_goldhandgun.png"; break;
	case ItemID::Weapon_GoldAssaultRifle: path = "Images/Items/item_goldassaultrifle.png"; break;
	case ItemID::Aid:                 path = "Images/Items/item_aid.png"; break;
	case ItemID::GreenSpray:          path = "Images/Items/item_greenspray.png"; break;
	case ItemID::PurpleSpray:         path = "Images/Items/item_purplespray.png"; break;
	case ItemID::OrangeSpray:         path = "Images/Items/item_orangespray.png"; break;
	case ItemID::RedSpray:            path = "Images/Items/item_redspray.png"; break;
	}

	return Director::getInstance()->getTextureCache()->addImage(path);
}
