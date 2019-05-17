#pragma once
#include "cocos2d.h"
#include "ItemFactory.hpp"
#include <array>

class ObjectLayer;
class ItemLayer;
class GUILayer;
class MapLayer;
class TutorialLayer;

class ControllLayer : public cocos2d::Layer
{
private:
	ControllLayer() = default;

	ObjectLayer* objectLayer;
	ItemLayer* itemLayer;
	GUILayer* guiLayer;
	MapLayer* mapLayer;
	TutorialLayer* tutorialLayer;

	int itemIndex;
	int weaponIndex;

	void SetItem(int index, bool soundFlag = true);
	void SetWeapon(int index, bool soundFlag = true);

	static bool isRunningUpdate;

public:
	static ControllLayer* create(ItemLayer* itemLayer, ObjectLayer* objectLayer, GUILayer* guiLayer, MapLayer* mapLayer, TutorialLayer* tutorialLayer);
	bool init(ItemLayer* itemLayer, ObjectLayer* objectLayer, GUILayer* guiLayer, MapLayer* mapLayer, TutorialLayer* tutorialLayer);
	virtual void update(float delta) override;

	inline int GetItemIndex() const { return itemIndex; }
	inline int GetWeaponIndex() const { return weaponIndex; }

	inline static void SetUpdate(bool flag) { isRunningUpdate = flag; }
};
