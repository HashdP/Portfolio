#include "MyScene.hpp"
#include "InputManager.hpp"
#include "GameManager.hpp"

USING_NS_CC;

bool MyScene::init()
{
	if (!Scene::init())
		return false;

	this->scheduleUpdate();

	InputManager::getInstance()->Init(this);

	Sprite* overlay = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Images/overlay.png"));
	overlay->setPosition(Director::getInstance()->getVisibleSize() / 2);
	overlay->setGlobalZOrder(10000);
	this->addChild(overlay);
	GameManager::getInstance()->SetOverlay(overlay);
	
	overlay->runAction(FadeOut::create(2.0f));

	return true;
}

void MyScene::update(float delta)
{
	InputManager::getInstance()->Update();
}
