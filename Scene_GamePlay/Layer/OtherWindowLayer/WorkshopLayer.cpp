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

	//�����̈ꗗ���j���[���쐬����
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

		Label* name = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 18), "�H�H�H");
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

	//�E���̐������j���[���쐬����
	Sprite* backRightBox = Sprite::create("Images/GUI/box_horror_grbl.png");
	backRightBox->setRotation(-90);
	backRightBox->setScale(3.5);
	backRightBox->setPositionX(400);
	backRightBox->setColor(Color3B::RED);
	this->addChild(backRightBox);

	Texture2D* itemBoxTex = Director::getInstance()->getTextureCache()->addImage("Images/GUI/item_box.png");

	//����A�C�e��
	Sprite* createItemImageBox = Sprite::createWithTexture(itemBoxTex);
	createItemImageBox->setScale(2.0f);
	createItemImageBox->setPosition(Vec2(380, 300));
	this->addChild(createItemImageBox);

	createItemImage = Sprite::createWithTexture(ItemFactory::getInstance()->GetItemTexture(createItem[0].itemID));
	createItemImage->setPosition(createItemImageBox->getContentSize() / 2);
	createItemImageBox->addChild(createItemImage);

	//�K�v�ȃA�C�e��
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

		Label* mul = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 20), "�~");
		mul->setPositionX(380);
		needItemNode->addChild(mul);

		Label* value = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 40), "0");
		value->setPositionX(450);
		value->setName("needValue");
		needItemNode->addChild(value);

		needItem.push_back(NeedItem(needItemNode));
	}

	//�쐻�{�^��
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

	Label* createButtonLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 20), "�쐬");
	createButtonLabel->setPosition(createButton->getContentSize() / 2);
	createButtonLabel->setName("label");
	createButton->addChild(createButtonLabel);

	return true;
}

void WorkshopLayer::update(float delta)
{
	//�쐻�{�^���������������ꍇ
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

	//�E�����̐����摜��ύX
	createItemImage->setTexture(ItemFactory::getInstance()->GetItemTexture(createItem[index].itemID));

	//����ɕK�v�ȃA�C�e����ύX
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

	//�f�ނ����ׂď������Ă��邩�m�F����
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

	//�쐬����󂫂��L�邩�m�F
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

	//�{�^�����������Ƃ��̃A�j���[�V����
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
	case ItemID::Aid: return "�񕜖�";
	case ItemID::Bullet: return "�e�e";
	case ItemID::Weapon_HandGun: return "�n���h�K��";
	case ItemID::Weapon_GoldHandGun: return "���̃n���h�K��";
	case ItemID::Weapon_AssaultRifle:return "�A�T���g���C�t��";
	case ItemID::Weapon_GoldAssaultRifle: return "���̃A�T���g���C�t��";
	case ItemID::GreenSpray: return "�΂̏����X�v���[";
	case ItemID::PurpleSpray: return "���̏����X�v���[";
	case ItemID::OrangeSpray: return "��̏����X�v���[";
	case ItemID::RedSpray: return "�Ԃ̏����X�v���[";
	}

	return "";
}