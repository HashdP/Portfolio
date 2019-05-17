#include "PunchBound.h"
#include "ObjectLayer.hpp"
#include "Player.hpp"
#include "Attack_Punch.hpp"
#include "ItemFactory.hpp"
#include "myutil.hpp"
#include "StateAnimationSprite.hpp"
#include "MoveAI.hpp"
#include "SoundManager.hpp"
#include "CharaData.hpp"

USING_NS_CC;

PunchBound* PunchBound::create(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData)
{
	PunchBound* pRet = new(std::nothrow) PunchBound();
	if (pRet && pRet->init(objectLayer, charaID, level, charaData))
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

bool PunchBound::init(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData)
{
	if (!Bound::init(objectLayer, charaID, level, charaData, random<float>(0.5f, 2.5f), random<float>(7.5f, 17.5f)))
		return false;

	float animspeed = 0.1f;
	for (int i = 0; i < (int)Direction::Num; ++i)
	{
		Direction dir = (Direction)i;
		std::string stateKey = "Attack_" + myutil::GetDirectionStr(dir);

		Animation* atkAnim = nullptr;
		switch (dir)
		{
		case Direction::Front:
			atkAnim = myutil::CreateStateAnimation("Images/Character/bound.png", animspeed, 3, 3, 64, 64);
			myutil::AddStateAnimation(atkAnim, "Images/Character/bound.png", 9, 2, 64, 64);
			GetSprite()->RegisterStateAnimation(stateKey, false, atkAnim, [this](Node*) { Freeze(0.5f); });
			break;

		case Direction::Back:
			atkAnim = myutil::CreateStateAnimation("Images/Character/bound.png", animspeed, 3, 3, 64, 64);
			myutil::AddStateAnimation(atkAnim, "Images/Character/bound.png", 11, 2, 64, 64);
			GetSprite()->RegisterStateAnimation(stateKey, false, atkAnim, [this](Node*) { Freeze(0.5f); });
			break;

		case Direction::Left:
			atkAnim = myutil::CreateStateAnimation("Images/Character/bound.png", animspeed, 3, 2, 64, 64);
			myutil::AddStateAnimation(atkAnim, "Images/Character/bound.png", 13, 3, 64, 64);
			GetSprite()->RegisterStateAnimation(stateKey, false, atkAnim, [this](Node*) { Freeze(0.5f); });
			break;

		case Direction::Right:
			atkAnim = myutil::CreateStateAnimation("Images/Character/bound.png", animspeed, 3, 2, 64, 64);
			myutil::AddStateAnimation(atkAnim, "Images/Character/bound.png", 13, 3, 64, 64);
			GetSprite()->RegisterStateAnimation(stateKey, true, atkAnim, [this](Node*) { Freeze(0.5f); });
			break;
		}

		GetSprite()->SetAnimFunc(
			stateKey,
			animspeed * 3,
			[this, dir, charaData](Node*)
		{
			SoundManager::getInstance()->Play2DSound(SoundID::Bound_Move);

			Attack_Punch* punch = Attack_Punch::create(GetCharaID(), GetObjectLayer(), getPosition(), dir, charaData["PunchAtk"], charaData["PunchFrz"], charaData["PunchKb"], SoundID::HitSound_BoundPunch);
			GetObjectLayer()->AddFieldObject(punch);
		});
	}

	SetColor(Color3B::GREEN);

	return true;
}

void PunchBound::CharacterUpdate(float delta)
{
	if (GetState() != "Attack")
	{
		const Player& player = GetObjectLayer()->GetConstPlayer();
		Direction dir = myutil::GetTargetDir(this->getPosition(), player.getPosition());

		Rect rect = Rect();

		switch (dir)
		{
		case Direction::Back:  rect.setRect(getPosition().x - 1, getPosition().y, 2, 20); break;
		case Direction::Left:  rect.setRect(getPosition().x - 20, getPosition().y - 1, 20, 2); break;
		case Direction::Front: rect.setRect(getPosition().x - 1, getPosition().y - 20, 2, 20); break;
		case Direction::Right: rect.setRect(getPosition().x, getPosition().y - 1, 20, 2); break;
		}

		if (rect.intersectsRect(player.GetHitBox()))
		{

			SetDirection(dir);
			SetState("Attack");
			return;
		}

		if (GetState() == "Move")
		{
			Vec2 dif = AIUpdate(delta, player.getPosition());
			MoveCharacter(dif * 0.8f);
		}

		SetState("Move");
	}
}

void PunchBound::OnRemove()
{
	if (GetBossFlag())
	{
		//ボスの場合の特別ドロップ
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::RedBall, 1);
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::IronScrap, 20);
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::GrainOfGold, 1);
	}
	else
	{
		switch (GetLevel())
		{
		case 1:
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::GreenBall, random<int>(1, 2));
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::Branch, random<int>(0, 3));
			break;

		case 2:
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::GreenBall, random<int>(1, 5));
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::Branch, random<int>(0, 6));
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::GrainOfGold, random<int>(0, 4) / 4);
			break;

		case 3:
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::GreenBall, random<int>(3, 8));
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::Branch, random<int>(4, 10));
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::GrainOfGold, random<int>(0, 2) / 2);
			break;
		}
	}
}