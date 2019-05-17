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

	//�A�C�e�����쐬����
	std::unique_ptr<Item> CreateItem(ItemID itemID, int num);

	//�h���b�v�A�C�e���i�t�B�[���h�ɗ����Ă���A�C�e���j���쐬����B����|�W�V����
	DropItem* CreateDropItem(ObjectLayer* objectLayer, const cocos2d::Vec2& position, ItemID itemID, int num);

	//�h���b�v�A�C�e���i�t�B�[���h�ɗ����Ă���A�C�e���j���쐬����B�t���A�̂ǂ���
	DropItem* CreateDropItemOnFloor(ObjectLayer* objectLayer, const cocos2d::Vec2& position, ItemID itemID, int num);

	//�A�C�e���̃e�N�X�`�����擾
	cocos2d::Texture2D* GetItemTexture(ItemID itemID);
};
