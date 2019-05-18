#include "GameObject/Character/Enemy/Bound/MagicBound.hpp"
#include "GameObject/Character/Player.hpp"
#include "Scene_GamePlay/HPGauge.hpp"
#include "Scene_GamePlay/Layer/ObjectLayer.hpp"
#include "Field/Area/Area.hpp"
#include "GameObject/Attack/Attack_FireBall.hpp"
#include "Factory/ItemFactory.hpp"
#include "StateAnimationSprite.hpp"
#include "Manager/SoundManager.hpp"
#include "Scene_GamePlay/CharaData.hpp"

USING_NS_CC;

MagicBound* MagicBound::create(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData)
{
	MagicBound* pRet = new(std::nothrow) MagicBound();
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

bool MagicBound::init(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData)
{
	if (!Bound::init(objectLayer, charaID, level, charaData, random<float>(0.5f, 2.5f), random<float>(7.5f, 17.5f)))
		return false;

	float animspeed = 0.1f;
	Animation* atkAnim = myutil::CreateStateAnimation("Images/Character/bound.png", animspeed, 3, 6, 64, 64);
	myutil::AddStateAnimation(atkAnim, "Images/Character/bound.png", 3, 6, 64, 64);
	myutil::AddStateAnimation(atkAnim, "Images/Character/bound.png", 3, 6, 64, 64);

	GetSprite()->RegisterStateAnimation("Attack", true, atkAnim, [this](Node*) { Freeze(0.5f); });

	GetSprite()->SetAnimFunc(
		"Attack",
		animspeed * 18,
		[this, charaData](Node*)
	{
		Vec2 baseDif = Vec2::ZERO;
		switch (GetDirection())
		{
		case Direction::Back:  baseDif = Vec2(0, 20); break;
		case Direction::Left:  baseDif = Vec2(-20, 0); break;
		case Direction::Front: baseDif = Vec2(0, -20);  break;
		case Direction::Right: baseDif = Vec2(20, 0);  break;
		}

		for (int i = 0; i < charaData["MagicNum"]; ++i)
		{
			Vec2 dif;
			if (i == 0)      dif = baseDif;
			else if (i == 1) dif == Vec2(-baseDif.y, baseDif.x);
			else if (i == 2) dif == Vec2(baseDif.y, -baseDif.x);

			Attack_FireBall* shot = Attack_FireBall::create(GetCharaID(), GetObjectLayer(), getPosition() + dif, GetDirection(), charaData["MagicDmg"], 0.0f, 0.0f, SoundID::HitSound_Bullet);
			GetObjectLayer()->AddFieldObject(shot);
		}
	});

	SetColor(Color3B::ORANGE);

	deltatime = random<float>(0.0f, 3.0f);
	attackspan = 3.0f;

	return true;
}

void MagicBound::CharacterUpdate(float delta)
{
	if (GetState() != "Attack")
	{
		Direction dir = myutil::GetTargetDir(this->getPosition(), GetObjectLayer()->GetConstPlayer().getPosition());

		deltatime += delta;
		if (attackspan <= deltatime)
		{
			deltatime = 0.0f;
			attackspan = random<float>(3.0f, 10.0f);

			SetDirection(dir);
			SetState("Attack");
			return;
		}

		if (GetState() == "Move")
		{
			Vec2 dif = AIUpdate(delta, GetObjectLayer()->GetArea().GetRandomMoveablePlace());
			MoveCharacter(dif * 0.5f);
		}

		SetState("Move");
	}
}

void MagicBound::OnRemove()
{
	switch (GetLevel())
	{
	case 1:
		break;
	}

	switch (GetLevel())
	{
	case 1:
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::Branch, random<int>(1, 3));
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::IronScrap, random<int>(1, 3));
		break;

	case 2:
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::Branch, random<int>(1, 5));
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::IronScrap, random<int>(1, 5));
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::GrainOfGold, random<int>(0, 4) / 4);
		break;

	case 3:
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::Branch, random<int>(3, 8));
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::IronScrap, random<int>(3, 8));
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::GrainOfGold, random<int>(0, 2) / 2);
		break;
	}
}