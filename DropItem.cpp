#include "DropItem.hpp"
#include "ObjectLayer.hpp"
#include "Player.hpp"
#include "ItemLayer.hpp"
#include "ItemFactory.hpp"
#include "SoundManager.hpp"

USING_NS_CC;

DropItem* DropItem::create(ObjectLayer* objectLayer, ItemLayer* itemLayer, ItemID itemID, int num)
{
	DropItem* item = new(std::nothrow) DropItem();
	if (item && item->init(objectLayer, itemLayer, itemID, num))
	{
		item->autorelease();
		return item;
	}
	else
	{
		delete item;
		item = nullptr;
		return nullptr;
	}
}

bool DropItem::init(ObjectLayer* objectLayer, ItemLayer* itemLayer, ItemID itemID, int num)
{
	if (!GameObject::init(objectLayer))
		return false;

	this->itemLayer = itemLayer;

	item = ItemFactory::getInstance()->CreateItem(itemID, num);

	Sprite* itemSprite = item->GetItemImage();
	itemSprite->setScale(0.4f);
	this->addChild(itemSprite);

	itemSprite->runAction(
		RepeatForever::create(
			Sequence::create(
				MoveBy::create(1.0f, Vec2(0.0f, 3.0f)),
				MoveBy::create(1.0f, Vec2(0.0f, -3.0f)),
				NULL
			)
		)
	);

	particle = ParticleSystemQuad::create("drop_item.plist");
	particle->setAutoRemoveOnFinish(true);
	particle->resetSystem();
	particle->setScale(0.25f);
	particle->setPosition(0.0f, 0.0f);
	particle->setColor(Color3B::BLUE);
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	this->addChild(particle);

	return true;
}

void DropItem::update(float delta)
{
	if (this->getPosition().getDistance(GetObjectLayer()->GetConstPlayer().getPosition()) < 10.0f)
	{
		//アイテムを回収出来たらオブジェクトを削除する
		if (itemLayer->AddItem(item->GetID(), item->GetNum()))
		{
			SoundManager::getInstance()->Play2DSound(SoundID::GetItem);
			GetObjectLayer()->RemoveObject(this);
		}
	}
}