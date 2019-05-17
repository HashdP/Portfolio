#include "Attack_FireBall.hpp"
#include "EffectFactory.hpp"
#include "ObjectLayer.hpp"
#include "Player.hpp"

USING_NS_CC;

bool Attack_FireBall::init(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float dmg, float frz, float kb, SoundID hitSEID)
{
	if (!Attack::init(parentID, objectLayer, originPosition, direction, dmg, frz, kb, hitSEID))
		return false;

	particle = ParticleSystemQuad::create("Particle/fireball_particle.plist");
	particle->setAutoRemoveOnFinish(true);
	particle->resetSystem();
	particle->setScale(0.25f);
	particle->setPosition(0, 0);
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	this->addChild(particle);

	interval = -2.0f;
	lifetime = 10.0f;

	moveAI.SetData(3.0f, 5.0f);

	return true;
}

void Attack_FireBall::update(float delta)
{
	if (interval <= 0.0f)
	{
		//追尾モード
		Vec2 pos = getPosition() + particle->getPosition();

		if (pos.x < -320.0f || 320.0f < pos.x || pos.y < -80.0f || 80.0f < pos.y || lifetime <= 0.0f)
		{
			if (GetDirection() == Direction::Back && 0.0f < lifetime)
				HitSparkle(pos);

			particle->stopSystem();
			interval += delta;
			return;
		}

		//ヒット判定
		if (HitAttack(pos, 6.0f))
			return;

		Vec2 dirVec = moveAI.MoveUpdate(delta, GetObjectLayer()->GetArea(), pos, GetObjectLayer()->GetConstPlayer().getPosition(), true);

		if (0 < dirVec.x) SetDirection(Direction::Right);
		else if (dirVec.x < 0) SetDirection(Direction::Left);
		else if (0 < dirVec.y) SetDirection(Direction::Back);
		else if (dirVec.y < 0) SetDirection(Direction::Front);

		if (interval == 0.0f)
		{
			//移動する
			particle->setPosition(particle->getPosition() + dirVec);
			lifetime -= delta;
		}
		else
		{
			//intervalがマイナスの間は待機する
			interval += delta;
			if (0 <= interval)
				interval = 0.0f;
		}
	}
	else
	{
		//１０秒経過後、３秒間のインターバル（パーティクルが消えるまで）を挟んで削除する
		if (3.0f < interval)
			GetObjectLayer()->RemoveObject(this);

		interval += delta;
	}
}

void Attack_FireBall::HitSparkle(const Vec2& position)
{
	EffectFactory::getInstance()->CreateEffect(GetObjectLayer(), EffectID::DirtShotHit)->setPosition(position);
}

float Attack_FireBall::GetFieldPositionY()
{
	if (interval <= 0.0f)
		return getPositionY() + particle->getPositionY();
	else
		return getPositionY();
}
