#pragma execution_character_set("utf-8")

#include "GameObject/Character/Enemy/NPC/Skull.hpp"
#include "Field/Area/Area.hpp"
#include "StateAnimationSprite.hpp"
#include "myutil.hpp"

USING_NS_CC;

bool Skull::init(ObjectLayer* objectLayer)
{
	if (!Character::init(objectLayer, CharacterID::Supervisor))
		return false;

	GetSprite()->RegisterStateAnimation("Stay", false, myutil::CreateStateAnimation("Images/Character/skull.png", 1.0f, 0, 1, 32, 32));

	SetGhost(true);
	GetSprite()->setAnchorPoint(Vec2(0.5f, 0.45f));
	SetState("Stay");

	return true;
}