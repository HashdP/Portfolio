#pragma execution_character_set("utf-8")

#include "TitleLayer.hpp"
#include "ObjectLayer.hpp"
#include "GUILayer.hpp"
#include "SoundManager.hpp"

USING_NS_CC;

TitleLayer* TitleLayer::create(ObjectLayer* objectLayer, GUILayer* guiLayer)
{
	TitleLayer* pRet = new(std::nothrow) TitleLayer();

	if (pRet && pRet->init(objectLayer, guiLayer))
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

bool TitleLayer::init(ObjectLayer* objectLayer, GUILayer* guiLayer)
{
	if (!Node::init())
		return false;

	this->setCascadeOpacityEnabled(true);

	Label* mainLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 80), "地下世界");
	mainLabel->setPositionY(220);
	this->addChild(mainLabel);

	//スタートボタン
	playButton = ui::Button::create("Images/GUI/mini_horror_gr.png", "", "");
	playButton->setCascadeOpacityEnabled(true);
	playButton->setPositionY(-120);
	playButton->setZoomScale(0.01f);
	playButton->addTouchEventListener(
		[this, objectLayer, guiLayer](Ref* sender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::BEGAN)
			ClickPlayButton(objectLayer, guiLayer);
	});
	this->addChild(playButton);

	Label* playLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 20), "PLAY");
	playLabel->setPosition(playButton->getContentSize() / 2);
	playButton->addChild(playLabel);

	//終了ボタン
	exitButton = ui::Button::create("Images/GUI/mini_horror_gr.png", "", "");
	exitButton->setCascadeOpacityEnabled(true);
	exitButton->setPositionY(-200);
	exitButton->setZoomScale(0.01f);
	exitButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			Director::getInstance()->end();
		}
	});
	this->addChild(exitButton);

	Label* exitLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 20), "EXIT");
	exitLabel->setPosition(exitButton->getContentSize() / 2);
	exitButton->addChild(exitLabel);

	objectLayer->setPositionY(objectLayer->getPositionY() - 280);
	guiLayer->setPositionY(guiLayer->getPositionY() - 280);

	objectLayer->StopUpdate();

	return true;
}

void TitleLayer::ClickPlayButton(ObjectLayer* objectLayer, GUILayer* guiLayer)
{
	Size display = Director::getInstance()->getVisibleSize();

	guiLayer->runAction(MoveTo::create(3.0f, display / 2));
	objectLayer->runAction(
		Sequence::create(
			MoveTo::create(3.0f, Vec2(objectLayer->getPositionX(), display.height + 80)),
			CallFuncN::create([objectLayer](Node*)
	{
		objectLayer->StartUpdate();
	}),
			NULL
		)
	);

	playButton->setEnabled(false);
	exitButton->setEnabled(false);
	this->runAction(
		Sequence::create(
			FadeOut::create(2.0f),
			CallFuncN::create([this](Node*) { this->removeFromParent(); }),
			NULL
		)
	);

	SoundManager::getInstance()->Play2DSound(SoundID::Title_Play);
}
