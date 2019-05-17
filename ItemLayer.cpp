#include "ItemLayer.hpp"
#include "GUILayer.hpp"
#include "ObjectLayer.hpp"
#include "ItemFactory.hpp"
#include "SoundManager.hpp"
#include "SaveData.hpp"

USING_NS_CC;

ItemLayer* ItemLayer::create(GUILayer* guiLayer)
{
	ItemLayer* pRet = new(std::nothrow) ItemLayer();
	if (pRet && pRet->init(guiLayer))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool ItemLayer::init(GUILayer* guiLayer)
{
	if (!Layer::init())
		return false;

	this->guiLayer = guiLayer;

	for (int i = 0; i < 24; ++i)
		items[i] = ItemFactory::getInstance()->CreateItem(ItemID::None, 0);
	for (int i = 0; i < 3; ++i)
		weapons[i] = ItemFactory::getInstance()->CreateItem(ItemID::None, 0);

	if (SaveData::getInstance()->LoadContinueFlag())
	{
		//セーブデータが存在する場合
		std::array<Item, 24> items = SaveData::getInstance()->LoadItem();
		for (int i = 0; i < 24; ++i)
			CreateItem(items[i].GetID(), items[i].GetNum(), i);

		std::array<Item, 3> weapons = SaveData::getInstance()->LoadWeapon();
		for (int i = 0; i < 3; ++i)
		{
			AddWeapon(weapons[i].GetID(), i);
		}
	}
	else
	{
		int endingNumber = SaveData::getInstance()->LoadEndingNumber();
		switch (endingNumber)
		{
		case 0:
			AddItem(ItemID::Aid, 2);
			AddItem(ItemID::GreenSpray, 1);
			break;

		case 1:
			AddItem(ItemID::Aid, 3);
			AddItem(ItemID::GreenSpray, 1);
			AddItem(ItemID::PurpleSpray, 1);
			AddItem(ItemID::Weapon_HandGun, 1);
			AddItem(ItemID::Bullet, 40);
			break;

		case 2:
			AddItem(ItemID::Aid, 4);
			AddItem(ItemID::GreenSpray, 1);
			AddItem(ItemID::PurpleSpray, 1);
			AddItem(ItemID::OrangeSpray, 1);
			AddItem(ItemID::Weapon_HandGun, 1);
			AddItem(ItemID::Weapon_AssaultRifle, 1);
			AddItem(ItemID::Bullet, 99);
			break;

		case 3:
			AddItem(ItemID::Aid, 5);
			AddItem(ItemID::PurpleSpray, 1);
			AddItem(ItemID::OrangeSpray, 1);
			AddItem(ItemID::RedSpray, 1);
			AddItem(ItemID::Weapon_HandGun, 1);
			AddItem(ItemID::Weapon_AssaultRifle, 1);
			AddItem(ItemID::Bullet, 198);
			AddItem(ItemID::GrainOfGold, 1);
			break;
		}

		AddWeapon(ItemID::None, 0);
		AddWeapon(ItemID::None, 1);
		AddWeapon(ItemID::None, 2);
	}

	return true;
}

bool ItemLayer::IsExistItem(ItemID itemID, int num)
{
	//itemIDをnumだけ所持しているならtrue
	for (unsigned i = 0; i < items.size(); ++i)
	{
		if (itemID == items[i]->GetID())
		{
			num -= items[i]->GetNum();

			if (num <= 0)
				return true;
		}
	}

	//numだけ所持していなかったら
	return false;
}

bool ItemLayer::IsExistItemEmpty(ItemID itemID, int num)
{
	int total = 0;
	for (unsigned i = 0; i < items.size(); ++i)
	{
		if (items[i]->GetID() == ItemID::None)
		{
			return true;
		}
		else if (itemID == items[i]->GetID())
		{
			total += items[i]->GetMaxNum() - items[i]->GetNum();

			if (num <= total)
				return true;
		}
	}

	//空きがない場合
	return false;
}

void ItemLayer::CreateItem(ItemID itemID, int num, int index)
{
	items[index] = ItemFactory::getInstance()->CreateItem(itemID, num);
	if (itemID != ItemID::None)
		guiLayer->AddItemImage(items[index]->GetItemImage(), index);
}

bool ItemLayer::AddItem(ItemID itemID, int num)
{
	if (itemID == ItemID::None)
		return false;

	//手持ちに入りきらないとき
	if (!IsExistItemEmpty(itemID, num))
		return false;

	//まず少しの空きに入れる
	for (unsigned i = 0; i < items.size(); ++i)
	{
		if (itemID == items[i]->GetID() && items[i]->GetNum() < items[i]->GetMaxNum())
		{
			int empty = items[i]->GetMaxNum() - items[i]->GetNum();
			int insert = num <= empty ? num : empty;

			items[i]->SetNum(items[i]->GetNum() + insert);
			num -= empty;

			if (num <= 0)
				return true;
		}
	}

	//新しくアイテムを作成
	for (unsigned i = 0; i < items.size(); ++i)
	{
		if (items[i]->GetID() == ItemID::None)
		{
			CreateItem(itemID, num, i);
			num -= items[i]->GetNum();

			if (num <= 0)
				return true;
		}
	}

	return true;
}

bool ItemLayer::RemoveItem(ItemID itemID, int num)
{
	for (int i = items.size() - 1; 0 <= i; --i)
	{
		if (items[i]->GetID() == itemID)
		{
			int remain = num - items[i]->GetNum();
			items[i]->SetNum(items[i]->GetNum() - num);
			num = remain;

			if (items[i]->GetNum() <= 0)
			{
				RemoveItem(i);
			}

			if (num <= 0)
				return true;
		}
	}

	return false;
}

void ItemLayer::RemoveItem(int index)
{
	if (items[index]->GetID() != ItemID::None)
	{
		items[index]->GetItemImage()->removeFromParent();
		items[index] = ItemFactory::getInstance()->CreateItem(ItemID::None, 0);
	}
}

void ItemLayer::SetItem(int index, bool playSound)
{
	guiLayer->SetItemBoxLight(index);
	if (playSound)
		SoundManager::getInstance()->Play2DSound(SoundID::ChangeItemCursor);
}

void ItemLayer::UseItem(int itemIndex, int weaponIndex, ObjectLayer* objectLayer)
{
	if (items[itemIndex]->Use(itemIndex, weaponIndex, this, objectLayer))
	{
		SoundManager::getInstance()->Play2DSound(SoundID::UseItem);
		items[itemIndex]->SetNum(items[itemIndex]->GetNum() - 1);

		if (items[itemIndex]->GetNum() == 0)
		{
			RemoveItem(itemIndex);
		}
	}
	else
	{
		SoundManager::getInstance()->Play2DSound(SoundID::CannotUseItem);
	}
}

void ItemLayer::SetWeapon(int index, bool playSound)
{
	guiLayer->SetWeaponBoxLight(index);
	if (playSound)
		SoundManager::getInstance()->Play2DSound(SoundID::ChangeWeaponCursor);
}

void ItemLayer::AddWeapon(ItemID itemID, int index)
{
	weapons[index] = ItemFactory::getInstance()->CreateItem(itemID, 1);

	if (itemID != ItemID::None)
	{
		guiLayer->AddWeaponImage(weapons[index]->GetItemImage(false), index);
	}
}

void ItemLayer::EquipWeapon(int itemIndex, int weaponIndex)
{
	ItemID weaponID = items[itemIndex]->GetID();

	//アイテム欄に戻す
	AddItem(weapons[weaponIndex]->GetID(), 1);

	//武器を装備する
	if (weapons[weaponIndex]->GetID() != ItemID::None)
		weapons[weaponIndex]->GetItemImage()->removeFromParent();
	AddWeapon(weaponID, weaponIndex);
}