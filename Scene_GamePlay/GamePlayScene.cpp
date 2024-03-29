#include "GamePlayScene.hpp"
#include "ObjectLayer.hpp"
#include "GUILayer.hpp"
#include "MapLayer.hpp"
#include "WorkshopLayer.hpp"
#include "TutorialLayer.hpp"
#include "TitleLayer.hpp"
#include "ItemLayer.hpp"
#include "ControllLayer.hpp"
#include "DropItem.hpp"
#include "Player.hpp"
#include "MyScene.hpp"
#include "ItemFactory.hpp"
#include "SoundManager.hpp"
#include "SaveData.hpp"

USING_NS_CC;

Scene* GamePlayScene::create()
{
	Size display = Director::getInstance()->getVisibleSize();

	SoundManager::getInstance()->LoadSound();
	SoundManager::getInstance()->Play2DSound(SoundID::Background, true);

	auto scene = MyScene::create();

	//GUIレイヤー
	GUILayer* GUILayer = GUILayer::create();
	GUILayer->setPosition(display.width / 2, display.height / 2);
	scene->addChild(GUILayer, 2);

	//アイテムレイヤー
	ItemLayer* itemLayer = ItemLayer::create(GUILayer);
	scene->addChild(itemLayer);
	ItemFactory::getInstance()->SetItemLayer(itemLayer);

	//工房
	WorkshopLayer* workshopLayer = WorkshopLayer::create(itemLayer);
	workshopLayer->setPosition(display.width / 2, display.height / 2);
	scene->addChild(workshopLayer, 1);

	//マップレイヤー
	MapLayer* mapLayer = MapLayer::create();
	mapLayer->setPosition(display.width / 2, display.height / 2);
	scene->addChild(mapLayer, 3);

	//オブジェクトレイヤー（レイヤーを２倍しているので実際のフィールドは640×360、フィールドの中心を(0,0)にする）
	ObjectLayer* objectLayer = ObjectLayer::create(itemLayer, workshopLayer);
	objectLayer->setScale(2.0f);
	objectLayer->setPosition(display.width, display.height + 80);
	scene->addChild(objectLayer, 0);

	//操作説明レイヤー
	TutorialLayer* tutorialLayer = TutorialLayer::create();
	tutorialLayer->setPosition(display.width / 2, display.height / 2);
	scene->addChild(tutorialLayer, 4);

	//コントローラー
	ControllLayer* controllLayer = ControllLayer::create(itemLayer, objectLayer, GUILayer, mapLayer, tutorialLayer);
	scene->addChild(controllLayer);

	//セーブデータの初期化
	SaveData::getInstance()->init(itemLayer, mapLayer);

	Point interruptionPoint = SaveData::getInstance()->LoadInterruptionPoint();
	Vec2 playerPosition = SaveData::getInstance()->LoadPlayerPosition();
	objectLayer->CreateField(playerPosition, mapLayer->GetMap(interruptionPoint), true);
	objectLayer->GetPlayer().SetGUILayer(GUILayer);
	if (interruptionPoint == Point::ZERO)
	{
		objectLayer->GetPlayer().SetDirection(Direction::Back);
	}
	else
	{
		if(playerPosition.x < 0)
			objectLayer->GetPlayer().SetDirection(Direction::Right);
		else
			objectLayer->GetPlayer().SetDirection(Direction::Left);
	}

	//タイトル
	TitleLayer* titleLayer = TitleLayer::create(objectLayer, GUILayer);
	titleLayer->setPosition(display.width / 2, display.height / 2);
	scene->addChild(titleLayer, 5);

	return scene;
}
