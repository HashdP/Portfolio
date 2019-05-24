#pragma execution_character_set("utf-8")

#include "TutorialLayer.hpp"
#include "SoundManager.hpp"

USING_NS_CC;

bool TutorialLayer::init()
{
	if (!OtherWindowLayer::init(SoundID::Activate_Map))
		return false;

	Size size = Director::getInstance()->getVisibleSize();

	//背景
	Sprite* background = Sprite::create("Images/GUI/GUIBackground.png");
	background->setColor(Color3B::BLUE);
	this->addChild(background);

	//操作説明
	Label* mainTitle = Label::createWithTTF(TTFConfig("Fonts/PixelMplus10-Regular.ttf", 60), "操作説明");
	mainTitle->setPosition(0, size.height / 2 - 80);
	this->addChild(mainTitle);

	std::vector<std::pair<std::string, std::string>> howto_texts;
	howto_texts.emplace_back("WASD", "歩く");
	howto_texts.emplace_back("WASD+SHIFT", "走る");
	howto_texts.emplace_back("左クリック", "攻撃／アイテムの選択");
	howto_texts.emplace_back("E", "選択中のアイテムを使用／インタラクト");
	howto_texts.emplace_back("C", "選択中のアイテムを捨てる");
	howto_texts.emplace_back("Q", "回復する（回復薬を所持している場合）");
	howto_texts.emplace_back("マウスホイール", "武器を変更");
	howto_texts.emplace_back("M", "マップを開く");

	for (unsigned i = 0; i < howto_texts.size(); ++i)
	{
		float y = size.height / 2 - 180 - 65 * i;

		Label* howto_key = Label::createWithTTF(TTFConfig("Fonts/PixelMplus10-Regular.ttf", 40), howto_texts[i].first);
		howto_key->setPosition(-320, y);
		this->addChild(howto_key);

		Label* howto_sec = Label::createWithTTF(TTFConfig("Fonts/PixelMplus10-Regular.ttf", 40), "：");
		howto_sec->setPosition(-160, y);
		this->addChild(howto_sec);

		Label* howto_des = Label::createWithTTF(TTFConfig("Fonts/PixelMplus10-Regular.ttf", 40), howto_texts[i].second);
		howto_des->setAnchorPoint(Vec2(0.0f, 0.5f));
		howto_des->setPosition(-140, y);
		this->addChild(howto_des);
	}

	return true;
}
