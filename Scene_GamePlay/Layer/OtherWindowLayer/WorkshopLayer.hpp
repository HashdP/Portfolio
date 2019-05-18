#pragma once
#include "Scene_GamePlay/Layer/OtherWindowLayer/OtherWindowLayer.hpp"
#include "ui/CocosGUI.h"
#include "Item/Item.hpp"
#include <array>

class ItemLayer;

class WorkshopLayer : public OtherWindowLayer
{
private:
	struct NeedItem
	{
		NeedItem(cocos2d::Node* node) : node(node), itemID(ItemID::None) {}

		ItemID itemID;
		int num;
		cocos2d::Node* node;
	};

	WorkshopLayer() = default;

	ItemLayer* itemLayer;

	cocos2d::Sprite* background;

	int currentIndex;

	std::vector<NeedItem> createItem;

	cocos2d::Sprite* createItemImage;

	std::vector<NeedItem> needItem;

	cocos2d::ui::Button* createButton;

	bool isPressCreate;
	float pressingDelta;
	float pressingCreateInterval;

	void SetNeedItem(ItemID itemID, int num);
	void SetCreateItem(ItemID itemID, int num);

	bool SelectCreateItem(int index);
	bool PressCreateButton();

	void OnVisible(ObjectLayer& objectLayer) override;

public:
	static WorkshopLayer* create(ItemLayer* itemLayer);
	virtual bool init(ItemLayer* itemLayer);
	virtual void update(float delta);

	static std::string GetCreateItemName(ItemID itemID);
};
