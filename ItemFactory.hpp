#pragma once
#include "cocos2d.h"
#include "Item.hpp"

class DropItem;
class ObjectLayer;
class ItemLayer;

class ItemFactory
{
private:
	ItemFactory() = default;
	~ItemFactory() = default;

	static ItemFactory* instance;

	ItemLayer* itemLayer;

public:
	ItemFactory(const ItemFactory& obj) = delete;
	ItemFactory& operator=(const ItemFactory& obj) = delete;
	ItemFactory(const ItemFactory&& obj) = delete;
	ItemFactory& operator=(const ItemFactory&& obj) = delete;

	static ItemFactory* getInstance();

	inline void SetItemLayer(ItemLayer* itemLayer) { this->itemLayer = itemLayer; }

	//アイテムを作成する
	std::unique_ptr<Item> CreateItem(ItemID itemID, int num);

	//ドロップアイテム（フィールドに落ちているアイテム）を作成する。特定ポジション
	DropItem* CreateDropItem(ObjectLayer* objectLayer, const cocos2d::Vec2& position, ItemID itemID, int num);

	//ドロップアイテム（フィールドに落ちているアイテム）を作成する。フロアのどこか
	DropItem* CreateDropItemOnFloor(ObjectLayer* objectLayer, const cocos2d::Vec2& position, ItemID itemID, int num);

	//アイテムのテクスチャを取得
	cocos2d::Texture2D* GetItemTexture(ItemID itemID);
};
