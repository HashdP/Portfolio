#include "EndingScene.hpp"
#include "MyScene.hpp"
#include "EndingLayer.hpp"

USING_NS_CC;

Scene* EndingScene::create(int endingNumber)
{
	MyScene* scene = MyScene::create();

	Size size = Director::getInstance()->getVisibleSize();

	EndingLayer* endingLayer = EndingLayer::create(endingNumber);
	endingLayer->setPosition(size / 2);
	scene->addChild(endingLayer);

	return scene;
}
