#include "ShotBound.hpp"
#include "Attack_DirtShot.hpp"
#include "ObjectLayer.hpp"
#include "Player.hpp"
#include "myutil.hpp"
#include "ItemFactory.hpp"
#include "StateAnimationSprite.hpp"
#include "SoundManager.hpp"
#include "CharaData.hpp"

USING_NS_CC;

ShotBound* ShotBound::create(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData)
{
	ShotBound* pRet = new(std::nothrow) ShotBound();
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

bool ShotBound::init(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData)
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
			GetSprite()->RegisterStateAnimation(stateKey, false, atkAnim, [this](Node*) { Freeze(1.0f); });
			break;

		case Direction::Back:
			atkAnim = myutil::CreateStateAnimation("Images/Character/bound.png", animspeed, 3, 3, 64, 64);
			myutil::AddStateAnimation(atkAnim, "Images/Character/bound.png", 11, 2, 64, 64);
			GetSprite()->RegisterStateAnimation(stateKey, false, atkAnim, [this](Node*) { Freeze(1.0f); });
			break;

		case Direction::Left:
			atkAnim = myutil::CreateStateAnimation("Images/Character/bound.png", animspeed, 3, 2, 64, 64);
			myutil::AddStateAnimation(atkAnim, "Images/Character/bound.png", 13, 3, 64, 64);
			GetSprite()->RegisterStateAnimation(stateKey, false, atkAnim, [this](Node*) { Freeze(1.0f); });
			break;

		case Direction::Right:
			atkAnim = myutil::CreateStateAnimation("Images/Character/bound.png", animspeed, 3, 2, 64, 64);
			myutil::AddStateAnimation(atkAnim, "Images/Character/bound.png", 13, 3, 64, 64);
			GetSprite()->RegisterStateAnimation(stateKey, true, atkAnim, [this](Node*) { Freeze(1.0f); });
			break;
		}

		GetSprite()->SetAnimFunc(
			stateKey,
			animspeed * 3,
			[this, dir, charaData](Node*)
		{
			Attack_DirtShot* shot = Attack_DirtShot::create(GetCharaID(), GetObjectLayer(), getPosition(), dir, charaData["ShotSpd"], charaData["ShotDmg"], 0.2f, 20.0f, SoundID::HitSound_DirtShot);
			GetObjectLayer()->AddFieldObject(shot);
		});
	}

	SetColor(Color3B(200, 0, 200));

	return true;
}

void ShotBound::CharacterUpdate(float delta)
{
	if (GetState() != "Attack")
	{
		const Player& player = GetObjectLayer()->GetConstPlayer();
		Direction dir = myutil::GetTargetDir(this->getPosition(), player.getPosition());

		bool shotFlag = false;
		Vec2 dif = player.getPosition() - this->getPosition();
		dif.x = std::abs(dif.x);
		dif.y = std::abs(dif.y);
		float distance = player.getPosition().getDistance(getPosition());

		if (distance <= 300.0f)
		{
			switch (dir)
			{
			case Direction::Back:
			case Direction::Front:
				if (dif.x < 10.0f)
					shotFlag = true;
				break;

			case Direction::Right:
			case Direction::Left:
				if (dif.y < 10.0f)
					shotFlag = true;
				break;
			}
		}

		if (shotFlag)
		{
			SetDirection(dir);
			SetState("Attack");
			return;
		}

		if (GetState() == "Move")
		{
			Vec2 dif = AIUpdate(delta, player.getPosition());
			MoveCharacter(dif * 0.7f);
		}

		SetState("Move");
	}
}

void ShotBound::OnRemove()
{
	if (GetBossFlag())
	{
		//ボスの場合の特別ドロップ
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::PurpleBall, 1);
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::HandGunParts, 1);
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::IronScrap, 15);
	}
	else
	{
		switch (GetLevel())
		{
		case 1:
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::IronScrap, random<int>(2, 4));
			break;

		case 2:
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::IronScrap, random<int>(4, 8));
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::GrainOfGold, random<int>(0, 4) / 4);
			break;

		case 3:
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::IronScrap, random<int>(6, 10));
			ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::GrainOfGold, random<int>(0, 2) / 2);
			break;
		}
	}
}