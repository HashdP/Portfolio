#include "Scene_GamePlay/GamePlayScene.hpp"
#include "Scene_GamePlay/Layer/ObjectLayer.hpp"
#include "Scene_GamePlay/Layer/GUILayer.hpp"
#include "Scene_GamePlay/Layer/OtherWindowLayer/MapLayer.hpp"
#include "Scene_GamePlay/Layer/OtherWindowLayer/WorkshopLayer.hpp"
#include "Scene_GamePlay/Layer/OtherWindowLayer/TutorialLayer.hpp"
#include "Scene_GamePlay/Layer/TitleLayer.hpp"
#include "Scene_GamePlay/Layer/ItemLayer.hpp"
#include "MyScene.hpp"
#include "Scene_GamePlay/Layer/ControllLayer.hpp"
#include "GameObject/DropItem.hpp"
#include "Factory/ItemFactory.hpp"
#include "Manager/SoundManager.hpp"
#include "GameObject/Character/Player.hpp"
#include "SaveData.hpp"

USING_NS_CC;

Scene* GamePlayScene::create()
{
	Size display = Director::getInstance()->getVisibleSize();

	SoundManager::getInstance()->LoadSound();
	SoundManager::getInstance()->Play2DSound(SoundID::Background, true);

	auto scene = MyScene::create();

	//GUI���C���[
	GUILayer* GUILayer = GUILayer::create();
	GUILayer->setPosition(display.width / 2, display.height / 2);
	scene->addChild(GUILayer, 2);

	//�A�C�e�����C���[
	ItemLayer* itemLayer = ItemLayer::create(GUILayer);
	scene->addChild(itemLayer);
	ItemFactory::getInstance()->SetItemLayer(itemLayer);

	//�H�[
	WorkshopLayer* workshopLayer = WorkshopLayer::create(itemLayer);
	workshopLayer->setPosition(display.width / 2, display.height / 2);
	scene->addChild(workshopLayer, 1);

	//�}�b�v���C���[
	MapLayer* mapLayer = MapLayer::create();
	mapLayer->setPosition(display.width / 2, display.height / 2);
	scene->addChild(mapLayer, 3);

	//�I�u�W�F�N�g���C���[�i���C���[���Q�{���Ă���̂Ŏ��ۂ̃t�B�[���h��640�~360�A�t�B�[���h�̒��S��(0,0)�ɂ���j
	ObjectLayer* objectLayer = ObjectLayer::create(itemLayer, workshopLayer);
	objectLayer->setScale(2.0f);
	objectLayer->setPosition(display.width, display.height + 80);
	scene->addChild(objectLayer, 0);

	//����������C���[
	TutorialLayer* tutorialLayer = TutorialLayer::create();
	tutorialLayer->setPosition(display.width / 2, display.height / 2);
	scene->addChild(tutorialLayer, 4);

	//�R���g���[���[
	ControllLayer* controllLayer = ControllLayer::create(itemLayer, objectLayer, GUILayer, mapLayer, tutorialLayer);
	scene->addChild(controllLayer);

	//�Z�[�u�f�[�^�̏�����
	SaveData::getInstance()->init(itemLayer, mapLayer);

	Vec2 playerPosition = SaveData::getInstance()->LoadPlayerPosition();
	Point interruptionPoint = SaveData::getInstance()->LoadInterruptionPoint();
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

	//�^�C�g��
	TitleLayer* titleLayer = TitleLayer::create(objectLayer, GUILayer);
	titleLayer->setPosition(display.width / 2, display.height / 2);
	scene->addChild(titleLayer, 5);

	return scene;
}
