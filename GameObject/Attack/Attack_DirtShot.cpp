#include "Attack_DirtShot.hpp"
#include "EffectFactory.hpp"
#include "ObjectLayer.hpp"
#include "myutil.hpp"

USING_NS_CC;

Attack_DirtShot* Attack_DirtShot::create(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float speed, float dmg, float frz, float kb, SoundID hitSEID)
{
	Attack_DirtShot* pRet = new(std::nothrow) Attack_DirtShot();
	if (pRet && pRet->init(parentID, objectLayer, originPosition, direction, speed, dmg, frz, kb, hitSEID))
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

bool Attack_DirtShot::init(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float speed, float dmg, float frz, float kb, SoundID hitSEID)
{
	if (!Attack::init(parentID, objectLayer, originPosition, direction, dmg, frz, kb, hitSEID))
		return false;

	particle = ParticleSystemQuad::create("Particle/dirtshot_particle.plist");
	particle->setAutoRemoveOnFinish(true);
	particle->resetSystem();
	particle->setScale(0.25f);
	particle->setPosition(0, 0);
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	this->addChild(particle);

	deleteInterval = 0.0f;

	this->speed = speed;

	return true;
}

void Attack_DirtShot::update(float delta)
{
	if (deleteInterval == 0.0f)
	{
		Vec2 pos = getPosition() + particle->getPosition();

		if (HitAttack(pos, 10.0f))
			return;

		particle->setPosition(particle->getPosition() + myutil::GetDirectionVec(GetDirection()) * speed);

		//ƒqƒbƒg”»’è
		if (pos.x < -320.0f || 320.0f < pos.x || pos.y < -80.0f || 80.0f < pos.y)
		{
			if (GetDirection() == Direction::Back)
				HitSparkle(pos);

			particle->stopSystem();
			deleteInterval += delta;
			return;
		}
	}
	else
	{
		if (3.0f < deleteInterval)
			GetObjectLayer()->RemoveObject(this);

		deleteInterval += delta;
	}
}

void Attack_DirtShot::HitSparkle(const Vec2& position)
{
	EffectFactory::getInstance()->CreateEffect(GetObjectLayer(), EffectID::DirtShotHit)->setPosition(position);
}

float Attack_DirtShot::GetFieldPositionY()
{
	if (deleteInterval == 0.0f)
		return getPositionY();
	else
		return getPositionY() + particle->getPositionY();
}
