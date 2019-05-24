#pragma execution_character_set("utf-8")

#include "GUILayer.hpp"
#include "HPGauge.hpp"

USING_NS_CC;

GUILayer* GUILayer::create()
{
	GUILayer* guiLayer = new(std::nothrow) GUILayer();

	if (guiLayer && guiLayer->init())
	{
		guiLayer->autorelease();
		return guiLayer;
	}
	else
	{
		delete guiLayer;
		guiLayer = nullptr;
		return nullptr;
	}
}

bool GUILayer::init()
{
	if (!Layer::init())
		return false;

	Texture2D* baseBoxTex = Director::getInstance()->getTextureCache()->addImage("Images/GUI/box_horror_grbl.png");
	Texture2D* boxLightTex = Director::getInstance()->getTextureCache()->addImage("Images/item_box_light.png");

	guiBackground = ui::Button::create("Images/GUI/box_horror_grbl.png", "", "");
	guiBackground->setScale(2.0f);
	guiBackground->setColor(Color3B(50, 105, 50));
	guiBackground->setPosition(Vec2(0, -270));
	guiBackground->setZoomScale(0.0f);
	this->addChild(guiBackground);

	Sprite* box = Sprite::createWithTexture(baseBoxTex);
	box->setScale(1.4f);
	box->setPosition(0, -230);
	this->addChild(box);

	//アイテムボックス
	Label* itemBoxLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 32), "ITEM");
	itemBoxLabel->setPosition(Vec2(0, -130));
	itemBoxLabel->setAnchorPoint(Vec2(0, 0.5f));
	this->addChild(itemBoxLabel);

	for (int i = 0; i < 24; ++i)
	{
		itemBoxes[i] = ui::Button::create("Images/GUI/item_box.png", "", "");
		itemBoxes[i]->setPosition(GetItemBoxPosition(i));
		itemBoxes[i]->setZoomScale(0.0f);
		itemBoxes[i]->setScale(1.1f);
		this->addChild(itemBoxes[i]);
	}

	itemBoxLight = Sprite::createWithTexture(boxLightTex);
	itemBoxLight->setScale(0.91666667f);
	this->addChild(itemBoxLight, 1);

	//装備ボックス
	for (int i = 0; i < 3; ++i)
	{
		weponBoxes[i] = ui::Button::create("Images/GUI/item_box.png", "", "");
		weponBoxes[i]->setPosition(GetWeaponBoxPosition(i));
		weponBoxes[i]->setScale(1.6f);
		weponBoxes[i]->setColor(Color3B(255, 150, 150));
		weponBoxes[i]->setZoomScale(0.0f);
		this->addChild(weponBoxes[i]);
	}

	weaponBoxLight = Sprite::createWithTexture(boxLightTex);
	weaponBoxLight->setScale(1.3333333f);
	this->addChild(weaponBoxLight, 1);

	//HPゲージ
	Node* hpGauge = CreateGauge(HPGauge, Color3B::GREEN);
	hpGauge->setPosition(-270, -170);
	hpGauge->setScale(2.3f, 0.8f);
	this->addChild(hpGauge);

	/*
	説明テキスト
	*/
	Label* hpLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 32), "HP");
	hpLabel->setPosition(-500, -130);
	hpLabel->setAnchorPoint(Vec2(0, 0.5f));
	this->addChild(hpLabel);

	Label* itemHowToLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 20), "（クリックで選択して、Eで使用）");
	itemHowToLabel->setPosition(Vec2(70, -130));
	itemHowToLabel->setAnchorPoint(Vec2(0, 0.5f));
	for (int i = 0; i < itemHowToLabel->getStringLength(); ++i)
	{
		//テキストを強調する
		if ((0 < i && i <= 4) || i == 11)
		{
			itemHowToLabel->getLetter(i)->setColor(Color3B::YELLOW);
			itemHowToLabel->getLetter(i)->setScale(1.2f);
		}
	}
	this->addChild(itemHowToLabel);

	Label* openTutorialLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 18), "（Tを押して操作説明を開く）");
	openTutorialLabel->setPosition(Vec2(240, -325));
	openTutorialLabel->setAnchorPoint(Vec2(0, 0.5f));
	openTutorialLabel->getLetter(1)->setColor(Color3B::YELLOW);
	openTutorialLabel->getLetter(1)->setScale(1.2f);
	this->addChild(openTutorialLabel);

	Label* weaponBoxLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 32), "WEAPON");
	weaponBoxLabel->setPosition(Vec2(-500, -230));
	weaponBoxLabel->setAnchorPoint(Vec2(0, 0.5f));
	this->addChild(weaponBoxLabel);

	Label* weaponHowToLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 20), "（マウスホイールで武器を変更）");
	weaponHowToLabel->setPosition(Vec2(-400, -230));
	weaponHowToLabel->setAnchorPoint(Vec2(0, 0.5f));
	for (int i = 0; i < weaponHowToLabel->getStringLength(); ++i)
	{
		//テキストを強調する
		if (0 < i && i <= 7)
		{
			weaponHowToLabel->getLetter(i)->setColor(Color3B::YELLOW);
			weaponHowToLabel->getLetter(i)->setScale(1.2f);
		}
	}
	this->addChild(weaponHowToLabel);

	return true;
}

void GUILayer::UpdateHPGauge(int hp, int maxhp)
{
	float scale = 1.0f * hp / maxhp;
	if (scale < 0.0f)
		scale = 0.0f;
	else if (1.0f < scale)
		scale = 1.0f;

	HPGauge->setScaleX(scale);
}

void GUILayer::SetGUIBackgroundFunc(const std::function<void(Ref* sender, ui::Widget::TouchEventType type)>& func)
{
	guiBackground->addTouchEventListener(func);
}
void GUILayer::SetItemBoxFunc(int index, const std::function<void(Ref* sender, ui::Widget::TouchEventType type)>& func)
{
	itemBoxes[index]->addTouchEventListener(func);
}
void GUILayer::SetWeaponBoxFunc(int index, const std::function<void(Ref* sender, ui::Widget::TouchEventType type)>& func)
{
	weponBoxes[index]->addTouchEventListener(func);
}

void GUILayer::AddItemImage(Sprite* sprite, int index)
{
	itemBoxes[index]->addChild(sprite);
	sprite->setPosition(itemBoxes[index]->getContentSize() / 2);
}

void GUILayer::SetItemBoxLight(int index)
{
	if (index == -1)
	{
		itemBoxLight->setVisible(false);
	}
	else
	{
		itemBoxLight->setVisible(true);
		itemBoxLight->setPosition(GetItemBoxPosition(index));
	}
}

void GUILayer::AddWeaponImage(Sprite* sprite, int index)
{
	weponBoxes[index]->addChild(sprite);
	sprite->setPosition(weponBoxes[index]->getContentSize() / 2);
}

void GUILayer::SetWeaponBoxLight(int index) 
{
	weaponBoxLight->setPosition(GetWeaponBoxPosition(index));
}

Node* GUILayer::CreateGauge(Sprite* &gaugeSprite, const Color3B& gaugeColor)
{
	Texture2D* gaugeBase_tex = Director::getInstance()->getTextureCache()->addImage("Images/GUI/wafuu_mini1_3.png");
	Texture2D* gauge_tex = Director::getInstance()->getTextureCache()->addImage("Images/GUI/button_blue_3.png");

	Node* gauge = Node::create();

	Sprite* gaugeBase = Sprite::createWithTexture(gaugeBase_tex);
	gauge->addChild(gaugeBase);

	gaugeSprite = Sprite::createWithTexture(gauge_tex);
	gaugeSprite->setColor(gaugeColor);
	gaugeSprite->setAnchorPoint(Vec2(1.f * 31 / 267, 0.5f));
	gaugeSprite->setPositionX(-102);
	gauge->addChild(gaugeSprite);

	Texture2D* gaugeBase_frame_tex = Director::getInstance()->getTextureCache()->addImage("Images/GUI/wafuu_mini1_2.png");
	Sprite* gaugeBase_frame = Sprite::createWithTexture(gaugeBase_frame_tex);
	gauge->addChild(gaugeBase_frame);

	return gauge;
}

Vec2 GUILayer::GetItemBoxPosition(int index)
{
	return Vec2(17 + 60 * (index % 8), -175 - 55 * (index / 8));
}

Vec2 GUILayer::GetWeaponBoxPosition(int index)
{
	return Vec2(-270 - 120 + index * 120, -290);
}
