#pragma execution_character_set("utf-8")

#include "Scene_GamePlay/Layer/OtherWindowLayer/WorkshopLayer.hpp"
#include "Factory/ItemFactory.hpp"
#include "Scene_GamePlay/Layer/ItemLayer.hpp"
#include "Manager/SoundManager.hpp"

USING_NS_CC;

WorkshopLayer* WorkshopLayer::create(ItemLayer* itemLayer)
{
	WorkshopLayer* disp = new (std::nothrow) WorkshopLayer();

	if (disp && disp->init(itemLayer))
	{
		disp->autorelease();
		return disp;
	}
	else
	{
		delete disp;
		disp = nullptr;
		return nullptr;
	}
}

bool WorkshopLayer::init(ItemLayer* itemLayer)
{
	if (!OtherWindowLayer::init(SoundID::Activate_Workshop))
		return false;

	scheduleUpdate();

	this->itemLayer = itemLayer;

	background = Sprite::create("Images/GUI/GUIBackground.png");
	background->setColor(Color3B::RED);
	this->addChild(background);

	//左側の一覧メニューを作成する
	Menu* leftMenu = Menu::create();
	leftMenu->setPositionX(-280);
	this->addChild(leftMenu);

	for (int i = 0; i < 10; ++i)
	{
		ui::Button* nameplate = ui::Button::create("Images/GUI/mini_horror_red.png", "", "");
		nameplate->setColor(Color3B(255, 200, 200));
		nameplate->setPosition(Vec2(-400 + 250 * (i / 5), 270 - 70 * (i % 5)));
		nameplate->setZoomScale(0.01f);
		nameplate->addTouchEventListener([this, i](Ref* sender, ui::Widget::TouchEventType type)
		{
			if (type == ui::Widget::TouchEventType::BEGAN)
			{
				SoundManager::getInstance()->Play2DSound(SoundID::ChangeItemCursor);
				SelectCreateItem(i);
			}
		});
		this->addChild(nameplate);

		Label* name = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 18), "？？？");
		name->setPosition(nameplate->getContentSize() / 2);
		name->setName("label");
		nameplate->addChild(name);

		createItem.push_back(NeedItem(nameplate));
	}

	SetCreateItem(ItemID::Aid, 1);
	SetCreateItem(ItemID::Bullet, 5);
	SetCreateItem(ItemID::Weapon_HandGun, 1);
	SetCreateItem(ItemID::Weapon_GoldHandGun, 1);
	SetCreateItem(ItemID::Weapon_AssaultRifle, 1);
	SetCreateItem(ItemID::Weapon_GoldAssaultRifle, 1);
	SetCreateItem(ItemID::GreenSpray, 1);
	SetCreateItem(ItemID::PurpleSpray, 1);
	SetCreateItem(ItemID::OrangeSpray, 1);
	SetCreateItem(ItemID::RedSpray, 1);

	//右側の説明メニューを作成する
	Sprite* backRightBox = Sprite::create("Images/GUI/box_horror_grbl.png");
	backRightBox->setRotation(-90);
	backRightBox->setScale(3.5);
	backRightBox->setPositionX(400);
	backRightBox->setColor(Color3B::RED);
	this->addChild(backRightBox);

	Texture2D* itemBoxTex = Director::getInstance()->getTextureCache()->addImage("Images/GUI/item_box.png");

	//制作アイテム
	Sprite* createItemImageBox = Sprite::createWithTexture(itemBoxTex);
	createItemImageBox->setScale(2.0f);
	createItemImageBox->setPosition(Vec2(380, 300));
	this->addChild(createItemImageBox);

	createItemImage = Sprite::createWithTexture(ItemFactory::getInstance()->GetItemTexture(createItem[0].itemID));
	createItemImage->setPosition(createItemImageBox->getContentSize() / 2);
	createItemImageBox->addChild(createItemImage);

	//必要なアイテム
	for (int y = 0; y < 3; y++)
	{
		Node* needItemNode = Node::create();
		needItemNode->setPositionY(200 - y * 70);
		needItemNode->setVisible(false);
		this->addChild(needItemNode);

		Sprite* needItemImageBox = Sprite::createWithTexture(itemBoxTex);
		needItemImageBox->setPositionX(300);
		needItemNode->addChild(needItemImageBox);

		Sprite* needItemImage = Sprite::createWithTexture(ItemFactory::getInstance()->GetItemTexture(ItemID::GreenBall));
		needItemImage->setPositionX(300);
		needItemImage->setName("item");
		needItemNode->addChild(needItemImage);

		Label* mul = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 20), "×");
		mul->setPositionX(380);
		needItemNode->addChild(mul);

		Label* value = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 40), "0");
		value->setPositionX(450);
		value->setName("needValue");
		needItemNode->addChild(value);

		needItem.push_back(NeedItem(needItemNode));
	}

	//作製ボタン
	createButton = ui::Button::create("Images/GUI/button_blue.png", "", "");
	createButton->setZoomScale(0);
	createButton->setPosition(Vec2(380, -20));
	createButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			pressingDelta = 0.0f;
			pressingCreateInterval = 0.0f;
			isPressCreate = PressCreateButton();
		}
		else if (type == ui::Widget::TouchEventType::ENDED || type == ui::Widget::TouchEventType::CANCELED)
		{
			isPressCreate = false;
		}
	});
	this->addChild(createButton);

	Label* createButtonLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 20), "作成");
	createButtonLabel->setPosition(createButton->getContentSize() / 2);
	createButtonLabel->setName("label");
	createButton->addChild(createButtonLabel);

	return true;
}

void WorkshopLayer::update(float delta)
{
	//作製ボタンを押し続けた場合
	if (isPressCreate)
	{
		if (pressingCreateInterval <= 0.0f)
		{
			if (1.0f <= pressingDelta && pressingDelta < 3.0f)
			{
				pressingCreateInterval = 0.1f;
			}
			else if (3.0f <= pressingDelta && pressingDelta < 5.0f)
			{
				pressingCreateInterval = 0.05f;
			}
			else if (5.0f <= pressingDelta)
			{
				pressingCreateInterval = 0.025f;
			}

			if (1.0f <= pressingDelta)
			{
				isPressCreate = PressCreateButton();
			}
		}

		pressingCreateInterval -= delta;
		pressingDelta += delta;
	}
}

void WorkshopLayer::SetCreateItem(ItemID itemID, int num)
{
	for (unsigned i = 0; i < createItem.size(); ++i)
	{
		if (createItem[i].itemID == ItemID::None)
		{
			createItem[i].itemID = itemID;
			createItem[i].num = num;
			createItem[i].node->getChildByName<Label*>("label")->setString(GetCreateItemName(itemID));
			break;
		}
	}
}

void WorkshopLayer::SetNeedItem(ItemID itemID, int num)
{
	for (NeedItem& nItem : needItem)
	{
		if (nItem.itemID == ItemID::None)
		{
			nItem.itemID = itemID;
			nItem.num = num;
			nItem.node->setVisible(true);

			nItem.node->getChildByName<Sprite*>("item")->setTexture(ItemFactory::getInstance()->GetItemTexture(itemID));
			nItem.node->getChildByName<Label*>("needValue")->setString(std::to_string(num));

			break;
		}
	}
}

bool WorkshopLayer::SelectCreateItem(int index)
{
	currentIndex = index;

	//右半分の説明画像を変更
	createItemImage->setTexture(ItemFactory::getInstance()->GetItemTexture(createItem[index].itemID));

	//制作に必要なアイテムを変更
	for (NeedItem& nItem : needItem)
	{
		nItem.itemID = ItemID::None;
		nItem.node->setVisible(false);
	}

	switch (createItem[index].itemID)
	{
	case ItemID::Weapon_HandGun:
		SetNeedItem(ItemID::HandGunParts, 1);
		SetNeedItem(ItemID::Branch, 10);
		SetNeedItem(ItemID::IronScrap, 10);
		break;

	case ItemID::Weapon_GoldHandGun:
		SetNeedItem(ItemID::Weapon_HandGun, 1);
		SetNeedItem(ItemID::GrainOfGold, 3);
		SetNeedItem(ItemID::IronScrap, 30);
		break;

	case ItemID::Weapon_AssaultRifle:
		SetNeedItem(ItemID::AssaultRifleParts, 1);
		SetNeedItem(ItemID::Branch, 30);
		SetNeedItem(ItemID::IronScrap, 30);
		break;

	case ItemID::Weapon_GoldAssaultRifle:
		SetNeedItem(ItemID::Weapon_AssaultRifle, 1);
		SetNeedItem(ItemID::GrainOfGold, 5);
		SetNeedItem(ItemID::IronScrap, 50);
		break;

	case ItemID::GreenSpray:
		SetNeedItem(ItemID::GreenBall, 5);
		break;

	case ItemID::PurpleSpray:
		SetNeedItem(ItemID::PurpleBall, 1);
		break;

	case ItemID::OrangeSpray:
		SetNeedItem(ItemID::OrangeBall, 1);
		break;

	case ItemID::RedSpray:
		SetNeedItem(ItemID::RedBall, 1);
		break;

	case ItemID::Bullet:
		SetNeedItem(ItemID::IronScrap, 1);
		break;

	case ItemID::Aid:
		SetNeedItem(ItemID::GreenBall, 1);
		SetNeedItem(ItemID::Branch, 2);
		break;
	}

	bool canCreate = true;

	//素材をすべて所持しているか確認する
	for (NeedItem& nItem : needItem)
	{
		if (nItem.itemID != ItemID::None)
		{
			if (!itemLayer->IsExistItem(nItem.itemID, nItem.num))
			{
				nItem.node->getChildByName<Label*>("needValue")->setColor(Color3B::RED);
				canCreate = false;
			}
			else
			{
				nItem.node->getChildByName<Label*>("needValue")->setColor(Color3B::WHITE);
			}
		}
	}

	//作成する空きが有るか確認
	if (canCreate)
		canCreate = itemLayer->IsExistItemEmpty(createItem[index].itemID, createItem[index].num);

	createButton->setEnabled(canCreate);
	createButton->setPressedActionEnabled(canCreate);
	if(canCreate)
		createButton->getChildByName("label")->setColor(Color3B::WHITE);
	else
		createButton->getChildByName("label")->setColor(Color3B::GRAY);

	return canCreate;
}

bool WorkshopLayer::PressCreateButton()
{
	for (NeedItem& nItem : needItem)
	{
		if (nItem.itemID != ItemID::None)
		{
			itemLayer->RemoveItem(nItem.itemID, nItem.num);
		}
	}

	//ボタンを押したときのアニメーション
	createButton->runAction(
		Sequence::create(
			ScaleTo::create(0.05f, 1.01f),
			ScaleTo::create(0.05f, 1.0f),
			NULL
		)
	);

	SoundManager::getInstance()->Play2DSound(SoundID::GetItem);
	itemLayer->AddItem(createItem[currentIndex].itemID, createItem[currentIndex].num);

	return SelectCreateItem(currentIndex);
}

void WorkshopLayer::OnVisible(ObjectLayer& objectLayer)
{
	SelectCreateItem(0);
}

std::string WorkshopLayer::GetCreateItemName(ItemID itemID)
{
	switch (itemID)
	{
	case ItemID::Aid: return "回復薬";
	case ItemID::Bullet: return "銃弾";
	case ItemID::Weapon_HandGun: return "ハンドガン";
	case ItemID::Weapon_GoldHandGun: return "金のハンドガン";
	case ItemID::Weapon_AssaultRifle:return "アサルトライフル";
	case ItemID::Weapon_GoldAssaultRifle: return "金のアサルトライフル";
	case ItemID::GreenSpray: return "緑の除染スプレー";
	case ItemID::PurpleSpray: return "紫の除染スプレー";
	case ItemID::OrangeSpray: return "橙の除染スプレー";
	case ItemID::RedSpray: return "赤の除染スプレー";
	}

	return "";
}