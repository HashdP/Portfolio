#include "GameManager.hpp"
#include "GamePlayScene.hpp"
#include "EndingScene.hpp"

USING_NS_CC;

GameManager* GameManager::instance = nullptr;

GameManager* GameManager::getInstance()
{
	if (!instance)
		instance = new GameManager();

	return instance;
}

void GameManager::ReplaceGamePlayScene()
{
	overlay->stopAllActions();
	overlay->runAction(
		Sequence::create(
			FadeIn::create(1.0f),
			DelayTime::create(1.0f),
			CallFuncN::create([](Node*) { Director::getInstance()->replaceScene(GamePlayScene::create()); }),
			NULL
		)
	);
}

void GameManager::ReplaceEndingScene(int endingNumber)
{
	overlay->stopAllActions();
	overlay->runAction(
		Sequence::create(
			FadeIn::create(1.0f),
			DelayTime::create(1.0f),
			CallFuncN::create([endingNumber](Node*) { Director::getInstance()->replaceScene(EndingScene::create(endingNumber)); }),
			NULL
		)
	);
}
