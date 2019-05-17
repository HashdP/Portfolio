#pragma execution_character_set("utf-8")

#include "Player.hpp"
#include "InputManager.hpp"
#include "ObjectLayer.hpp"
#include "GUILayer.hpp"
#include "ItemLayer.hpp"
#include "Route.hpp"
#include "myutil.hpp"
#include "StateAnimationSprite.hpp"
#include "Attack_Punch.hpp"
#include "EndingScene.hpp"
#include "SoundManager.hpp"
#include "Area.hpp"
#include "Floor.hpp"
#include "MapNode.hpp"
#include "Item_HandGun.hpp"
#include "Item_AssaultRifle.hpp"
#include "ItemFactory.hpp"
#include "CharaData.hpp"
#include "GameManager.hpp"

USING_NS_CC;

Player* Player::create(ObjectLayer* objectLayer, ItemLayer* itemLayer, const CharaData& charaData)
{
	Player* pRet = new(std::nothrow) Player();
	if (pRet && pRet->init(objectLayer, itemLayer, charaData))
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

bool Player::init(ObjectLayer* objectLayer, ItemLayer* itemLayer, const CharaData& charaData)
{
	if (!Character::init(objectLayer, CharacterID::Player, charaData))
		return false;

	for (int i = 0; i < (int)Direction::Num; ++i)
	{
		Direction dir = (Direction)i;

		int num = 0;
		switch (dir)
		{
		case Direction::Front: num = 0; break;
		case Direction::Back:  num = 1; break;
		case Direction::Left:  num = 2; break;
		case Direction::Right: num = 3; break;
		}

		Animation* stay = myutil::CreateStateAnimation("Images/Character/player.png", 0.5f, num * 2,     2, 32, 32);
		GetSprite()->RegisterStateAnimation("Stay_" + myutil::GetDirectionStr(dir), false, stay);

		Animation* walk = myutil::CreateStateAnimation("Images/Character/player.png", 0.2f, num * 6 + 8, 6, 32, 32);
		GetSprite()->RegisterStateAnimation("Walk_" + myutil::GetDirectionStr(dir), false, walk);

		Animation* run = myutil::CreateStateAnimation("Images/Character/player.png", 0.1f, num * 6 + 8, 6, 32, 32);
		GetSprite()->RegisterStateAnimation("Run_" + myutil::GetDirectionStr(dir), false, run);
	}
	GetSprite()->setAnchorPoint(Vec2(0.5f, 0.07f));

	hukidashi = nullptr;

	if (itemLayer)
	{
		for (int i = 0; i < 3; ++i)
		{
			ItemID weaponID = itemLayer->GetWeapon(i).GetID();

			switch (weaponID)
			{
			case ItemID::Weapon_HandGun:
			case ItemID::Weapon_GoldHandGun:
				static_cast<Item_HandGun*>(ItemFactory::getInstance()->CreateItem(weaponID, 1).get())->CreateAttackMotion(*this, "Attack" + std::to_string(i), itemLayer, objectLayer);
				break;

			case ItemID::Weapon_AssaultRifle:
			case ItemID::Weapon_GoldAssaultRifle:
				static_cast<Item_AssaultRifle*>(ItemFactory::getInstance()->CreateItem(weaponID, 1).get())->CreateAttackMotion(*this, "Attack" + std::to_string(i), itemLayer, objectLayer);
				break;

			default:
				RemoveWeapon(i);
				break;
			}
		}
	}

	return true;
}

void Player::CharacterUpdate(float delta)
{
	if (GetObjectLayer()->GetArea().IsMoveablePlace(getPosition()))
	{
		Floor* floor = GetObjectLayer()->GetArea().GetFloor(getPosition());
		if (floor->IsFloorEventActive() && floor->GetHukidashiText() != "")
		{
			if (hukidashi == nullptr)
			{
				hukidashi = Speak(floor->GetHukidashiText(), -1.0f);
			}
			else
			{
				if (hukidashi->getChildByName<Label*>("text")->getString() != floor->GetHukidashiText())
				{
					hukidashi->removeFromParent();
					hukidashi = Speak(floor->GetHukidashiText(), -1.0f);
				}
			}
		}
		else
		{
			if (hukidashi != nullptr)
			{
				hukidashi->removeFromParent();
				hukidashi = nullptr;
			}
		}
	}
}

void Player::SetState(std::string state, bool changeAnimation)
{
	if (GetState() != state)
	{
		SoundManager::getInstance()->StopSound(audioID);

		if (state == "Run")
		{
			audioID = SoundManager::getInstance()->Play2DSound(SoundID::Player_Run, true);
		}
		else if (state == "Walk")
		{
			audioID = SoundManager::getInstance()->Play2DSound(SoundID::Player_Walk, true);
		}
	}

	Character::SetState(state, changeAnimation);
}

void Player::SetHP(int HP)
{
	Character::SetHP(HP);
	guiLayer->UpdateHPGauge(GetHP(), GetMaxHP());

	if (HP <= 0)
	{
		GetObjectLayer()->StopUpdate();
		SoundManager::getInstance()->StopAllSound();
		GameManager::getInstance()->ReplaceEndingScene(0);
	}
}

void Player::MoveCharacter(Vec2& difPosition)
{
	//プレイヤーが移動する場合はエリア移動も考慮する
	Vec2 toPosition = getPosition() + difPosition;
	Vec2 floorPos = myutil::ConvertPosToFloorPos(getPosition());

	RouteDirection toDir = myutil::GetRouteDirectionFromFloorPos(floorPos);
	if (toDir != RouteDirection::RouteNum && GetObjectLayer()->GetCurrentMap().GetRoute(toDir).GetState() == RouteState::Normal)
	{
		if ((toPosition.x < -320 && InputManager::getInstance()->GetInput(InputCode::KEY_A) && (toDir == RouteDirection::LowerLeft || toDir == RouteDirection::UpperLeft)) ||
			(320 < toPosition.x && InputManager::getInstance()->GetInput(InputCode::KEY_D) && (toDir == RouteDirection::LowerRight || toDir == RouteDirection::UpperRight)))
		{
			//オーバーレイを作成
			Sprite* blackOverlay = Sprite::create("Images/overlay.png");
			blackOverlay->setOpacity(0);
			blackOverlay->setGlobalZOrder(10);
			blackOverlay->setPosition(Director::getInstance()->getVisibleSize() / 2);
			Director::getInstance()->getRunningScene()->addChild(blackOverlay);

			blackOverlay->runAction(
				Sequence::create(
					DelayTime::create(1.0f),
					FadeIn::create(1.0f),
					CallFuncN::create([this, toDir](Node* node) { GetObjectLayer()->MoveField(toDir); }),
					DelayTime::create(1.0f),
					FadeOut::create(1.0f),
					CallFuncN::create([this](Node* node) { GetObjectLayer()->StartUpdate(); }),
					NULL
				));
			GetObjectLayer()->StopUpdate();
			SetState("Run");

			this->runAction(MoveBy::create(2.0f, Vec2(getPosition().getNormalized().x, 0) * 1.5f * 120));

			SoundManager::getInstance()->StopSound(audioID);
			int dashSoundID = SoundManager::getInstance()->Play2DSound(SoundID::Player_Run, true);
			this->runAction(
				Sequence::create(
					DelayTime::create(3.0f),
					CallFuncN::create([dashSoundID](Node*) { SoundManager::getInstance()->StopSound(dashSoundID); }),
					NULL
				)
			);

			return;
		}
	}

	Character::MoveCharacter(difPosition);
}

void Player::RemoveWeapon(int weaponIndex)
{
	Texture2D* pAttackTex = Director::getInstance()->getTextureCache()->addImage("Images/Character/player_attack.png");

	auto endFunc = [this](Ref* ref) { SetState("Stay", false); };

	CharaData punchData("Player_Punch");

	for (int i = 0; i < (int)Direction::Num; ++i)
	{
		Direction dir = (Direction)i;

		std::string keyStr = "Attack" + std::to_string(weaponIndex) + "_" + myutil::GetDirectionStr(dir);

		switch (dir)
		{
		case Direction::Left:  GetSprite()->RegisterStateAnimation(keyStr, false, myutil::CreateStateAnimation("Images/Character/player_attack.png", punchData["Spd"], 10, 4, 32, 32), endFunc); break;
		case Direction::Right: GetSprite()->RegisterStateAnimation(keyStr, true, myutil::CreateStateAnimation("Images/Character/player_attack.png",  punchData["Spd"], 10, 4, 32, 32), endFunc); break;
		case Direction::Back:  GetSprite()->RegisterStateAnimation(keyStr, false, myutil::CreateStateAnimation("Images/Character/player_attack.png", punchData["Spd"], 5, 4, 32, 32), endFunc); break;
		case Direction::Front: GetSprite()->RegisterStateAnimation(keyStr, false, myutil::CreateStateAnimation("Images/Character/player_attack.png", punchData["Spd"], 0, 4, 32, 32), endFunc); break;
		}

		GetSprite()->SetAnimFunc(
			keyStr,
			0.0f,
			[this, dir, punchData](Node*)
		{
			SoundManager::getInstance()->Play2DSound(SoundID::Player_Punch);

			Attack_Punch* punch = Attack_Punch::create(GetCharaID(), GetObjectLayer(), getPosition(), dir, punchData["Atk"], punchData["Frz"], punchData["Kb"], SoundID::HitSound_PlayerPunch);
			punch->SetRange(12);
			GetObjectLayer()->AddFieldObject(punch);
		});
	}
}
