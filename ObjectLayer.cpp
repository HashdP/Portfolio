#pragma execution_character_set("utf-8")

#include "ObjectLayer.hpp"
#include "Player.hpp"
#include "Floor.hpp"
#include "Light.hpp"
#include "CharacterFactory.hpp"
#include "MapNode.hpp"
#include "Route.hpp"
#include "myutil.hpp"
#include "WorkshopLayer.hpp"
#include "ChangeAreaArrow.hpp"
#include "Wall_Base.hpp"
#include "Wall_Root.hpp"
#include "Wall_Boss.hpp"
#include "Area_Boss.hpp"
#include "Area_Base.hpp"
#include "SideFloor.hpp"
#include "Conversation.hpp"
#include "SaveData.hpp"
#include "ControllLayer.hpp"
#include "CharaData.hpp"

USING_NS_CC;

bool ObjectLayer::followPlayer = true;

ObjectLayer* ObjectLayer::create(ItemLayer* itemLayer, WorkshopLayer* workshopLayer)
{
	ObjectLayer* obj = new(std::nothrow) ObjectLayer();

	if (obj && obj->init(itemLayer, workshopLayer))
	{
		obj->autorelease();
		return obj;
	}
	else
	{
		delete obj;
		obj = nullptr;
		return nullptr;
	}
}

bool ObjectLayer::init(ItemLayer* itemLayer, WorkshopLayer* workshopLayer)
{
	if (!Layer::init())
		return false;

	this->scheduleUpdate();

	this->workshopLayer = workshopLayer;

	//プレイヤーを作成
	Player* player = Player::create(this, itemLayer, CharaData("Player"));
	AddFieldObject(player);

	for (int i = 0; i < RouteDirection::RouteNum; ++i)
	{
		ChangeAreaArrow* changeAreaArrow = ChangeAreaArrow::create(static_cast<RouteDirection>(i), RouteState::Normal);
		changeAreaArrow->setPosition(-380 + (i / 2) * 760, 64 - (i % 2) * 128);
		this->addChild(changeAreaArrow, INT_MAX);

		arrows.emplace_back(changeAreaArrow);
	}

	//オーバーレイ
	Sprite* overlay = Sprite::create("Images/overlay.png");
	overlay->setOpacity(200);
	this->addChild(overlay, INT_MAX - 1);

	Texture2D* sideOverlay = Director::getInstance()->getTextureCache()->addImage("Images/side_overlay.png");

	Sprite* sideOverlay_left = Sprite::createWithTexture(sideOverlay);
	sideOverlay_left->setPositionX(-480);
	this->addChild(sideOverlay_left, INT_MAX - 1);

	Sprite* sideOverlay_right = Sprite::createWithTexture(sideOverlay);
	sideOverlay_right->setFlippedX(true);
	sideOverlay_right->setPositionX(480);
	this->addChild(sideOverlay_right, INT_MAX - 1);

	//うっすら光らせる
	Light* areaLight = Light::create("Images/lightmask.png", 3);
	areaLight->setPosition(Vec2(0, -100));
	areaLight->setColor(Color3B(255, 200, 100));
	areaLight->setScale(4.0f);
	this->addChild(areaLight, INT_MAX);

	isRunningUpdate = true;

	return true;
}

void ObjectLayer::update(float delta)
{
	if (followPlayer)
	{
		this->setPositionX(1280 - player->getPositionX() * 2);
	}

	if (enemy.size() == 0)
	{
		//敵をすべて倒したとき、Dirtの道を解放する
		for (int i = 0; i < (int)RouteDirection::RouteNum; ++i)
		{
			if (currentMap->GetRoute((RouteDirection)i).GetState() == RouteState::Dirt)
			{
				ReleaseRoot((RouteDirection)i);
			}
		}
	}

	//オブジェクトをy座標で降順にソートする
	std::sort(fieldObjects.begin(), fieldObjects.end(),
		[](myutil::cocos_shared_ref<GameObject>& obj1, myutil::cocos_shared_ref<GameObject>& obj2) -> bool
	{
		return obj2->GetFieldPositionY() < obj1->GetFieldPositionY();
	});

	//キャラクターのzOrderを設定する
	int zOrder = 10;
	for (auto object : fieldObjects)
	{
		object->setLocalZOrder(zOrder++);
	}

	//オブジェクトを削除する
	if (0 < removeObjects.size())
	{
		for (unsigned i = 0; i < removeObjects.size(); ++i)
		{
			removeObjects[i]->removeFromParent();
		}

		removeObjects.clear();
	}
}

Vec2 ObjectLayer::DamageCharacter(CharacterID parentID, int damage, float freeze, float knockback, Direction direction, std::function<bool(const Character&)> func)
{
	std::vector<myutil::cocos_shared_ref<Character>> hitCharacters;

	if (parentID == CharacterID::Player)
	{
		for (const auto& chara : enemy)
		{
			if (!chara->IsGhost() && func(*chara))
			{
				hitCharacters.emplace_back(chara);
			}
		}
	}
	else
	{
		if (!player->IsGhost() && func(*player))
		{
			hitCharacters.emplace_back(player);
		}
	}

	if (0 < hitCharacters.size())
	{
		//ヒットしたキャラクターがいた場合
		myutil::cocos_shared_ref<Character> hitChara = hitCharacters[0];

		for (unsigned i = 1; i < hitCharacters.size(); ++i)
		{
			switch (direction)
			{
			case Direction::Back:
				if (hitCharacters[i]->getPositionY() < hitChara->getPositionY())
					hitChara = hitCharacters[i];
				break;
			case Direction::Front:
				if (hitChara->getPositionY() < hitCharacters[i]->getPositionY())
					hitChara = hitCharacters[i];
				break;
			case Direction::Right:
				if (hitCharacters[i]->getPositionX() < hitChara->getPositionX())
					hitChara = hitCharacters[i];
				break;
			case Direction::Left:
				if (hitChara->getPositionX() < hitCharacters[i]->getPositionX())
					hitChara = hitCharacters[i];
				break;
			}
		}

		hitChara->Damage(damage, freeze, myutil::GetDirectionVec(direction) * knockback);
		return hitChara->getPosition();
	}

	return Vec2(10000, 10000);
}

const MapNode ObjectLayer::GetCurrentMap() const
{
	return *currentMap;
}

void ObjectLayer::AddFieldObject(GameObject* fieldObject)
{
	myutil::cocos_shared_ref<GameObject> addObject = nullptr;

	if (fieldObject->GetObjectType() == "Player")
	{
		player = myutil::make_cocos_shared_ref(static_cast<Player*>(fieldObject));
		addObject = player;
	}
	else if(fieldObject->GetObjectType() == "Character" || fieldObject->GetObjectType() == "Enemy")
	{
		myutil::cocos_shared_ref<Character> character = myutil::make_cocos_shared_ref(static_cast<Character*>(fieldObject));
		enemy.emplace_back(character);
		character->SetState("Stay");
		addObject = character;
	}
	else
	{
		addObject = myutil::make_cocos_shared_ref(fieldObject);
	}

	if (addObject)
	{
		if (!isRunningUpdate)
			addObject->unscheduleUpdate();

		fieldObjects.emplace_back(addObject);
		this->addChild(fieldObject);
	}
}

void ObjectLayer::RemoveObject(const GameObject* gameObject)
{
	myutil::cocos_shared_ref<GameObject> removeObject = nullptr;

	//fieldObjectsの中から要素を取り除く
	for (unsigned i = 0; i < fieldObjects.size(); ++i)
	{
		if (gameObject == (fieldObjects[i]).get())
		{
			removeObject = fieldObjects[i];
			fieldObjects.erase(fieldObjects.begin() + i);
			break;
		}
	}

	//characterの中から要素を取り除く
	if (gameObject->GetObjectType() == "Enemy" || gameObject->GetObjectType() == "Character")
	{
		for (unsigned i = 0; i < enemy.size(); ++i)
		{
			if (gameObject == (enemy[i]).get())
			{
				enemy.erase(enemy.begin() + i);
				break;
			}
		}
	}

	if (removeObject)
	{
		if (removeObject->GetOnRemove())
			removeObject->OnRemove();
		removeObjects.emplace_back(removeObject);
	}
}

void ObjectLayer::MoveField(RouteDirection toDir)
{
	Vec2 toPosition = Vec2::ZERO;
	switch (toDir)
	{
	case RouteDirection::UpperLeft:  toPosition = area->GetFloor(FIELD_X_NUM - 1, 0)->getPosition(); break;
	case RouteDirection::LowerLeft:  toPosition = area->GetFloor(FIELD_X_NUM - 1, FIELD_Y_NUM - 1)->getPosition(); break;
	case RouteDirection::UpperRight: toPosition = area->GetFloor(0, 0)->getPosition(); break;
	case RouteDirection::LowerRight: toPosition = area->GetFloor(0, FIELD_Y_NUM - 1)->getPosition(); break;
	}

	CreateField(toPosition, currentMap->GetRoute(toDir).GetDestination(currentMap), false);
}

void ObjectLayer::CreateField(const cocos2d::Vec2& playerPosition, std::shared_ptr<MapNode> currentMap, bool firstCrate)
{
	this->currentMap = currentMap;

	//プレイヤー以外のオブジェクトを破棄
	for (int i = fieldObjects.size() - 1; 0 <= i; --i)
	{
		if (fieldObjects[i]->GetObjectType() != "Player")
		{
			fieldObjects[i]->SetOnRemove(false);
			RemoveObject(fieldObjects[i].get());
		}
	}

	this->removeChild(wall);
	this->removeChild(area);
	this->removeChild(sideFloor);

	//端の装飾床
	sideFloor = SideFloor::create(*currentMap);
	this->addChild(sideFloor, 1);

	//壁と床
	if (currentMap->GetLengthFromBase() == 0)
	{
		wall = Wall_Base::create();
		area = Area_Base::create(*currentMap, this, workshopLayer);
	}
	else if (currentMap->GetBossID() != CharacterID::None)
	{
		wall = Wall_Boss::create();
		area = Area_Boss::create(*currentMap, this, currentMap->GetRouteLevel());
	}
	else
	{
		wall = Wall_Root::create();
		area = Area_Root::create(*currentMap);
	}
	this->addChild(wall, 0);
	this->addChild(area, 2);

	//移動矢印を変更
	for (int i = 0; i < RouteDirection::RouteNum; ++i)
	{
		const Route route = currentMap->GetRoute(static_cast<RouteDirection>(i));

		arrows[i]->ChangeState(route.GetState());

		int sideX = i / 2 * (FIELD_X_NUM - 1);
		int sideY = (1 - i % 2) * (FIELD_Y_NUM - 1);

		switch (route.GetState())
		{
		case RouteState::Dirt:
			if (currentMap->GetLengthFromBase() == 0)
				area->GetFloor(sideX, sideY)->SetFloorEvent("「" + WorkshopLayer::GetCreateItemName(ItemID::GreenSpray) + "」を「左クリック」して「E」で使用");
			else
				area->GetFloor(sideX, sideY)->SetFloorEvent("汚れがひどい。敵をすべて倒そう。");;
			break;
		case RouteState::HeavyDirt:     area->GetFloor(sideX, sideY)->SetFloorEvent("「" + WorkshopLayer::GetCreateItemName(ItemID::PurpleSpray) + "」が必要だ。"); break;
		case RouteState::BadDirt:       area->GetFloor(sideX, sideY)->SetFloorEvent("「" + WorkshopLayer::GetCreateItemName(ItemID::OrangeSpray) + "」が必要だ。"); break;
		case RouteState::DangerousDirt: area->GetFloor(sideX, sideY)->SetFloorEvent("「" + WorkshopLayer::GetCreateItemName(ItemID::RedSpray) + "」が必要だ。"); break;
		}
	}

	player->setPosition(playerPosition);
	this->setPositionX(1280 - playerPosition.x * 2);

	//ベースにいるならば、NPCを一人追加する
	if (currentMap->GetLengthFromBase() == 0)
	{
		Character* npc = CharacterFactory::getInstance()->CreateNpc(this, CharacterID::Supervisor, myutil::ConvertFloorPosToPos(12, 4));

		if (firstCrate)
		{
			//一番最初のCreateでは、監視員が喋る
			npc->runAction(
				Sequence::create(
					DelayTime::create(3.0f),
					CallFuncN::create([npc](Node*)
			{
				int endingNumber = SaveData::getInstance()->LoadEndingNumber();
				Conversation* conv = Conversation::create("Text/supervisor" + std::to_string(endingNumber + 1) + ".txt", npc, 3.0f);
				npc->addChild(conv);
			}),
					NULL
				)
			);
		}
	}

	//敵を作成
	for (int i = 0; i < currentMap->GetEnemyNum(); ++i)
	{
		Character* enemy = CharacterFactory::getInstance()->CreateNpc(this, currentMap->GetThisStageRandomEnemy(), area->GetRandomMoveablePlace());
		enemy->Freeze(random<float>(1.0f, 3.0f));
	}

	//ボスがいる場合はボスを作成する
	if (currentMap->GetBossID() != CharacterID::None)
	{
		CharacterFactory::getInstance()->CreateNpc(this, currentMap->GetBossID(), area->GetRandomMoveablePlace(), true);
	}

	player->SetState("Stay");

	//セーブする
	if (currentMap->GetPosition() == Point::ZERO)
		SaveData::getInstance()->SavePlayerPosition(myutil::ConvertFloorPosToPos(12, 3));
	else
		SaveData::getInstance()->SavePlayerPosition(playerPosition);
	SaveData::getInstance()->SaveInterruptionPoint(currentMap->GetPosition());
	SaveData::getInstance()->SaveWeapon();
	SaveData::getInstance()->SaveItem();
	SaveData::getInstance()->SaveMap();
}

void ObjectLayer::StopUpdate()
{
	if (player != nullptr)
		player->SetState("Stay");

	for (auto object : fieldObjects)
	{
		object->unscheduleUpdate();
	}

	ControllLayer::SetUpdate(false);
	followPlayer = false;
	isRunningUpdate = false;
}

void ObjectLayer::StartUpdate()
{
	for (auto object : fieldObjects)
	{
		object->scheduleUpdate();
	}

	ControllLayer::SetUpdate(true);
	followPlayer = true;
	isRunningUpdate = true;
}

void ObjectLayer::ReleaseRoot(RouteDirection RouteDirection)
{
	arrows[static_cast<int>(RouteDirection)]->ChangeState(RouteState::Normal);

	currentMap->SetRouteState(RouteDirection, RouteState::Normal);

	int dirInt = static_cast<int>(RouteDirection);
	area->GetFloor(dirInt / 2 * (FIELD_X_NUM - 1), (1 - dirInt % 2) * (FIELD_Y_NUM - 1))->ClearFloorEvent();
}