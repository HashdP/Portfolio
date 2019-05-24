#include "Bound.hpp"
#include "HPGauge.hpp"
#include "StateAnimationSprite.hpp"
#include "SoundManager.hpp"
#include "myutil.hpp"
#include "CharaData.hpp"

USING_NS_CC;

bool Bound::init(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData, float searchInterval, float nextRootRange)
{
	if (!Enemy::init(objectLayer, charaID, charaData, searchInterval, nextRootRange))
		return false;

	this->level = level;

	GetSprite()->RegisterStateAnimation("Stay", false, myutil::CreateStateAnimation("Images/Character/bound.png", 0.3f, 0, 3, 64, 64));
	GetSprite()->RegisterStateAnimation("Move", false, myutil::CreateStateAnimation("Images/Character/bound.png", 0.2f, 3, 6, 64, 64));

	SetStateSound("Move", SoundComponent::create(SoundID::Bound_Move, 1.2f, 1.2f));

	GetSprite()->setAnchorPoint(Vec2(0.5f, 0.42f));

	switch (level)
	{
	default:
		GetSprite()->setScale(0.8f);
		break;

	case 2:
		GetSprite()->setScale(1.0f);
		break;

	case 3:
		GetSprite()->setScale(1.2f);
		break;
	}

	SetState("Stay");

	return true;
}