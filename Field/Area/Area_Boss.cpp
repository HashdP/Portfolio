#pragma execution_character_set("utf-8")

#include "Field/Area/Area_Boss.hpp"
#include "Scene_GamePlay/Floor.hpp"
#include "Scene_GamePlay/MoveAI.hpp"
#include "Scene_GamePlay/Layer/ObjectLayer.hpp"
#include "Scene_GamePlay/MapNode.hpp"
#include "Scane_Ending/EndingScene.hpp"
#include "Manager/SoundManager.hpp"
#include "Manager/GameManager.hpp"

USING_NS_CC;

Area_Boss* Area_Boss::create(MapNode& mapNode, ObjectLayer* objectLayer, int level)
{
	Area_Boss* pRet = new(std::nothrow) Area_Boss();
	if (pRet && pRet->init(mapNode, objectLayer, level))
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

bool Area_Boss::init(MapNode& mapNode, ObjectLayer* objectLayer, int level)
{
	if (!Area_Route::init(mapNode))
		return false;

	this->objectLayer = objectLayer;
	this->level = level;

	return true;
}

void Area_Boss::InitializeFloor()
{
	Area_Route::InitializeFloor();

	//ボスの場合、中央に脱出するはしごがある
	for (int y = FIELD_Y_NUM - 1; 0 <= y; --y)
	{
		if (GetFloor(10, y)->GetFloorID() == FloorID::Floor_1)
			break;

		CreateFloor(FloorID::Floor_1, 10, y);
	}
}

void Area_Boss::CreateFloorEvent()
{
	GetFloor(10, 4)->SetFloorEvent(
		"「脱出する」",
		[this]()
	{
		objectLayer->StopUpdate();
		SoundManager::getInstance()->StopAllSound();
		SoundManager::getInstance()->Play2DSound(SoundID::ClimbLadder);
		GameManager::getInstance()->ReplaceEndingScene(level);
		return true;
	},
		[this]() { return objectLayer->IsExistEnemy(); }
	);
}
