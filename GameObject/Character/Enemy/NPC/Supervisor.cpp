#pragma execution_character_set("utf-8")

#include "Supervisor.hpp"
#include "Area.hpp"
#include "StateAnimationSprite.hpp"
#include "myutil.hpp"

USING_NS_CC;

bool Supervisor::init(ObjectLayer* objectLayer)
{
	if (!Character::init(objectLayer, CharacterID::Supervisor))
		return false;

	GetSprite()->RegisterStateAnimation("Stay_Front", false, myutil::CreateStateAnimation("Images/Character/supervisor.png", 0.5f, 0, 2, 32, 32));
	GetSprite()->RegisterStateAnimation("Stay_Left", false, myutil::CreateStateAnimation("Images/Character/supervisor.png", 0.5f, 2, 2, 32, 32));
	GetSprite()->RegisterStateAnimation("Stay_Right", true, myutil::CreateStateAnimation("Images/Character/supervisor.png", 0.5f, 2, 2, 32, 32));
	GetSprite()->setAnchorPoint(Vec2(0.5f, 0.07f));

	SetGhost(true);
	SetDirection(Direction::Front);
	SetState("Stay");

	return true;
}