#pragma once
#include "GameObject.hpp"
#include "Item.hpp"

class ItemLayer;

class DropItem : public GameObject
{
private:
	DropItem() = default;

	ItemLayer* itemLayer;

	std::unique_ptr<Item> item;
	cocos2d::ParticleSystemQuad* particle;

public:
	virtual ~DropItem() = default;
	static DropItem* create(ObjectLayer* objectLayer, ItemLayer* itemLayer, ItemID itemID, int num);
	virtual bool init(ObjectLayer* objectLayer, ItemLayer* itemLayer, ItemID itemID, int num);
	virtual void update(float delta) override;

	/*
	ObjectType‚ðŽæ“¾
	*/
	inline std::string GetObjectType() const final { return "Item"; }
};
