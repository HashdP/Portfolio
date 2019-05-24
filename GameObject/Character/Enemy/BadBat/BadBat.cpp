#include "BadBat.hpp"
#include "Player.hpp"
#include "HPGauge.hpp"
#include "ObjectLayer.hpp"
#include "Area.hpp"
#include "Attack_Bullet.hpp"
#include "ItemFactory.hpp"
#include "myutil.hpp"
#include "StateAnimationSprite.hpp"
#include "MoveAI.hpp"
#include "SoundManager.hpp"

USING_NS_CC;

BadBat* BadBat::create(ObjectLayer* objectLayer, const CharaData& charaData)
{
	BadBat* pRet = new(std::nothrow) BadBat();
	if (pRet && pRet->init(objectLayer, charaData))
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

bool BadBat::init(ObjectLayer* objectLayer, const CharaData& charaData)
{
	if (!Enemy::init(objectLayer, CharacterID::BadBat, charaData, random<float>(0.5f, 2.5f), random<float>(7.5f, 17.5f)))
		return false;

	for (int i = 0; i < (int)Direction::Num; ++i)
	{
		Direction dir = (Direction)i;

		int startNum = 8;
		if (dir == Direction::Back)      startNum = 4;
		else if(dir == Direction::Front) startNum = 0;

		bool reverseFlag = dir == Direction::Right ? true : false;

		GetSprite()->RegisterStateAnimation("Stay_" + myutil::GetDirectionStr(dir), reverseFlag, myutil::CreateStateAnimation("Images/Character/badbat.png", 0.15f, startNum, 4, 64, 64));
		GetSprite()->RegisterStateAnimation("Move_" + myutil::GetDirectionStr(dir), reverseFlag, myutil::CreateStateAnimation("Images/Character/badbat.png", 0.1f, startNum, 4, 64, 64));

		float animspeed = 0.05f;

		Animation* atkAnim = myutil::CreateStateAnimation("Images/Character/badbat.png", 0.05f, startNum, 4, 64, 64);
		GetSprite()->RegisterStateAnimation("Attack_" + myutil::GetDirectionStr(dir), reverseFlag, atkAnim,
			[this](Node*) 
		{ 
			GetSprite()->ResetAnimationState();

			if (--attackCount == 0)
				Freeze(1.0f);
			else
				SetState("Attack");
		});

		GetSprite()->SetAnimFunc(
			"Attack_" + myutil::GetDirectionStr(dir),
			animspeed * 3,
			[this, dir](Node*)
		{
			SoundManager::getInstance()->Play2DSound(SoundID::BadBat_Shot);

			Attack_Bullet* bullet = Attack_Bullet::create(GetCharaID(), GetObjectLayer(), getPosition(), dir, 2, 0.3f, 10.0f, SoundID::HitSound_Bullet);
			bullet->setColor(Color3B::MAGENTA);
			GetObjectLayer()->AddFieldObject(bullet);
		});
	}

	//パーティクル
	ParticleSystemQuad* particle = ParticleSystemQuad::create("Particle/badbat_particle.plist");
	particle->setAutoRemoveOnFinish(true);
	particle->resetSystem();
	particle->setScale(0.5f);
	particle->setPosition(0, 20);
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	this->addChild(particle);

	GetSprite()->setAnchorPoint(Vec2(0.5f, 0.25f));
	GetSprite()->setScale(1.0f);

	return true;
}

void BadBat::CharacterUpdate(float delta)
{
	const Player& player = GetObjectLayer()->GetConstPlayer();
	Direction dir = myutil::GetTargetDir(this->getPosition(), player.getPosition());

	if (GetState() != "Attack")
	{
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

		if (shotFlag && attackInterval <= 0.0f)
		{
			attackCount = 3;
			attackInterval = random<float>(3.0f, 5.0f);

			SetDirection(dir);
			SetState("Attack");

			return;
		}
		attackInterval -= delta;

		if (GetState() == "Move")
		{
			Vec2 toPos = Vec2::ZERO;
			if (distance <= 150.0f)
				toPos = GetObjectLayer()->GetArea().GetRandomMoveablePlace();
			else
				toPos = player.getPosition();

			Vec2 dif = AIUpdate(delta, toPos, true, true);
			SetDirection(myutil::GetDirectionFromVec(dif));
			setPosition(getPosition() + dif * 1.0f);
		}

		SetState("Move");
	}
}

void BadBat::Damage(int damage, float freeze, const Vec2& knockBack)
{
	attackInterval = 0.0f;

	Enemy::Damage(damage, freeze, knockBack);
}

void BadBat::OnRemove()
{
	if (GetBossFlag())
	{
		//ボスの特別ドロップ
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::OrangeBall, 1);
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::AssaultRifleParts, 1);
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::Branch, 20);
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::IronScrap, 20);
	}
	else
	{
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::Branch, random<int>(0, 2));
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::Bullet, random<int>(5, 10));
		ItemFactory::getInstance()->CreateDropItemOnFloor(GetObjectLayer(), this->getPosition(), ItemID::GrainOfGold, random<int>(0, 10) / 10);
	}
}