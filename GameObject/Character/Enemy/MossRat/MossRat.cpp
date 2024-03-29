#include "MossRat.hpp"
#include "Player.hpp"
#include "HPGauge.hpp"
#include "ObjectLayer.hpp"
#include "Floor.hpp"
#include "Area.hpp"
#include "ItemFactory.hpp"
#include "myutil.hpp"
#include "Attack_Punch.hpp"
#include "Attack_FireBall.hpp"
#include "StateAnimationSprite.hpp"
#include "SoundManager.hpp"
#include "CharaData.hpp"

USING_NS_CC;

MossRat* MossRat::create(ObjectLayer* objectLayer, const CharaData& charaData)
{
	MossRat* pRet = new(std::nothrow) MossRat();
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

bool MossRat::init(ObjectLayer* objectLayer, const CharaData& charaData)
{
	if (!Enemy::init(objectLayer, CharacterID::MossRat, charaData, 1.0f, random<float>(7.5f, 17.5f)))
		return false;

	GetSprite()->RegisterStateAnimation("Stay_" + myutil::GetDirectionStr(Direction::Front), false, myutil::CreateStateAnimation("Images/Character/mossrat.png", 0.15f, 0, 2, 96, 96));
	GetSprite()->RegisterStateAnimation("Stay_" + myutil::GetDirectionStr(Direction::Back), false, myutil::CreateStateAnimation("Images/Character/mossrat.png", 0.15f, 2, 2, 96, 96));
	GetSprite()->RegisterStateAnimation("Stay_" + myutil::GetDirectionStr(Direction::Left), false, myutil::CreateStateAnimation("Images/Character/mossrat.png", 0.15f, 4, 2, 96, 96));
	GetSprite()->RegisterStateAnimation("Stay_" + myutil::GetDirectionStr(Direction::Right), true, myutil::CreateStateAnimation("Images/Character/mossrat.png", 0.15f, 4, 2, 96, 96));

	GetSprite()->RegisterStateAnimation("Move_" + myutil::GetDirectionStr(Direction::Front), false, myutil::CreateStateAnimation("Images/Character/mossrat.png", 0.1f, 6, 4, 96, 96));
	GetSprite()->RegisterStateAnimation("Move_" + myutil::GetDirectionStr(Direction::Back), false, myutil::CreateStateAnimation("Images/Character/mossrat.png", 0.1f, 10, 4, 96, 96));
	GetSprite()->RegisterStateAnimation("Move_" + myutil::GetDirectionStr(Direction::Left), false, myutil::CreateStateAnimation("Images/Character/mossrat.png", 0.1f, 14, 4, 96, 96));
	GetSprite()->RegisterStateAnimation("Move_" + myutil::GetDirectionStr(Direction::Right), true, myutil::CreateStateAnimation("Images/Character/mossrat.png", 0.1f, 14, 4, 96, 96));

	//歩く音
	SetStateSound("Move", SoundComponent::create(SoundID::MossRat_Move, 0.0f, 0.32f));

	//攻撃を作成する
	float atkSpeed = 0.1f;
	for (int i = 0; i < static_cast<int>(Direction::Num); ++i)
	{
		Direction dir = static_cast<Direction>(i);

		//攻撃１を作成する
		int startNum = 26;
		if (dir == Direction::Front) startNum = 18;
		else if (dir == Direction::Back) startNum = 22;

		bool reverseFlag = dir == Direction::Right;

		GetSprite()->RegisterStateAnimation("Attack_" + myutil::GetDirectionStr(dir), reverseFlag, myutil::CreateStateAnimation("Images/Character/mossrat.png", atkSpeed, startNum, 4, 96, 96), [this](Node*) { ChangeToType(); });

		GetSprite()->SetAnimFunc(
			"Attack_" + myutil::GetDirectionStr(dir),
			atkSpeed * 3,
			[this, dir, charaData](Node*)
		{
			SoundManager::getInstance()->Play2DSound(SoundID::MossRat_Attack);

			Attack_Punch* punch = Attack_Punch::create(GetCharaID(), GetObjectLayer(), getPosition(), dir, charaData["SlashDmg"], charaData["SlashFrz"], charaData["SlashKb"], SoundID::HitSound_MossRatPunch);
			GetObjectLayer()->AddFieldObject(punch);
		});

		//攻撃２を作成する
		if (dir == Direction::Right || dir == Direction::Left)
		{
			Animation* attack2Anim = myutil::CreateStateAnimation("Images/Character/mossrat.png", atkSpeed, 4, 2, 96, 96);
			for (int i = 0; i < 15; ++i) myutil::AddStateAnimation(attack2Anim, "Images/Character/mossrat.png", 4, 2, 96, 96);
			GetSprite()->RegisterStateAnimation("Attack2_" + myutil::GetDirectionStr(dir), reverseFlag, attack2Anim, [this](Node*) { Freeze(2.0f); });

			GetSprite()->SetAnimFunc(
				"Attack2_" + myutil::GetDirectionStr(dir),
				0.0f,
				[this, dir, charaData](Node*)
			{
				SoundManager::getInstance()->Play2DSound(SoundID::MossRat_Magic);

				ParticleSystemQuad* particle = ParticleSystemQuad::create("Particle/mossrat_attack2_particle.plist");
				particle->setAutoRemoveOnFinish(true);
				particle->resetSystem();
				particle->setScale(0.7f);
				particle->setPosition(0, 0);
				particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
				this->addChild(particle);

				this->runAction(
					Sequence::create(
						DelayTime::create(0.5f),
						CallFuncN::create(
							[this, charaData](Node*)
				{

					SoundManager::getInstance()->Play2DSound(SoundID::Bound_Magic);
					Attack_FireBall* shot = Attack_FireBall::create(GetCharaID(), GetObjectLayer(), getPosition() + Vec2(0, 20), GetDirection(), charaData["MagicDmg"], 0.3f, 10.0f, SoundID::HitSound_Bullet);
					GetObjectLayer()->AddFieldObject(shot);
				}),
						DelayTime::create(0.5f),
					CallFuncN::create(
						[this, charaData](Node*)
				{
					SoundManager::getInstance()->Play2DSound(SoundID::Bound_Magic);
					Attack_FireBall* shot = Attack_FireBall::create(GetCharaID(), GetObjectLayer(), getPosition() + Vec2(0, -20), GetDirection(), charaData["MagicDmg"], 0.3f, 10.0f, SoundID::HitSound_Bullet);
					GetObjectLayer()->AddFieldObject(shot);
				}),
					DelayTime::create(0.5f),
					CallFuncN::create(
						[this, charaData](Node*)
				{
					Vec2 dif = Vec2::ZERO;
					if (GetDirection() == Direction::Right)
						dif = Vec2(25, 0);
					else
						dif = Vec2(-25, 0);

					SoundManager::getInstance()->Play2DSound(SoundID::Bound_Magic);
					Attack_FireBall* shot = Attack_FireBall::create(GetCharaID(), GetObjectLayer(), getPosition() + dif, GetDirection(), charaData["MagicDmg"], 0.3f, 10.0f, SoundID::HitSound_Bullet);
					GetObjectLayer()->AddFieldObject(shot);
				}),
						NULL
					)
				);
			});
		}
	}
	GetSprite()->setAnchorPoint(Vec2(0.5f, 0.445f));
	GetSprite()->setScale(0.8f);

	actionType = 0;
	changeTypeProb = 0.0f;

	return true;
}

void MossRat::CharacterUpdate(float delta)
{
	if (GetState() != "Attack" && GetState() != "Attack2")
	{
		if (actionType == 0)
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
				Vec2 dif = AIUpdate(delta, player.getPosition(), false, true);
				SetDirection(myutil::GetDirectionFromVec(dif));
				setPosition(getPosition() + dif * 1.6f);
			}

			SetState("Move");
		}
		else if (actionType == 1)
		{
			if (GetMoveAI().IsArrive())
			{
				Vec2 floorPos = myutil::ConvertPosToFloorPos(type2ToPosition);
				if (floorPos.x == 0)
					SetDirection(Direction::Right);
				else
					SetDirection(Direction::Left);

				ChangeToType();

				SetState("Attack2");
				return;
			}

			if (GetState() == "Move")
			{
				Vec2 dif = AIUpdate(delta, type2ToPosition);
				SetDirection(myutil::GetDirectionFromVec(dif));
				setPosition(getPosition() + dif * 1.6f);
			}

			SetState("Move");
		}
	}
}

void MossRat::ChangeToType()
{
	if (actionType == 0)
	{
		if (random<float>(0.0f, 1.0f) < changeTypeProb)
			actionType = 1;
		else
			changeTypeProb += 0.1f;

		type2ToPosition = random<float>(0.0f, 1.0f) < 0.5f ? GetObjectLayer()->GetArea().GetFloor(0, 2)->getPosition() : GetObjectLayer()->GetArea().GetFloor(FIELD_X_NUM - 1, 2)->getPosition();

		SearchRoute(type2ToPosition);

		Freeze(1.0f);
	}
	else if (actionType == 1)
	{
		actionType = 0;
		changeTypeProb = 0.0f;
	}
}

void MossRat::Damage(int damage, float freeze, const cocos2d::Vec2& knockBack)
{
	if (GetState() == "Attack2")
	{
		//StateがAttack2の場合はFreezeしない
		Enemy::Damage(damage, 0.0f, Vec2::ZERO);
	}
	else
	{
		Enemy::Damage(damage, freeze, knockBack);
	}
}
