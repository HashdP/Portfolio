#include "Attack_Punch.hpp"
#include "EffectFactory.hpp"
#include "ObjectLayer.hpp"

USING_NS_CC;

bool Attack_Punch::init(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float dmg, float frz, float kb, SoundID hitSEID)
{
	if (!Attack::init(parentID, objectLayer, originPosition, direction, dmg, frz, kb, hitSEID))
		return false;

	SetRange(20);

	lifetime = 0.2f;

	return true;
}

void Attack_Punch::update(float delta)
{
	if (HitAttack(punchRect))
		return;

	if (0.0f < lifetime)
	{
		//timeLimit‚ªÝ’è‚³‚ê‚Ä‚¢‚éê‡AŽ©“®‚Åíœ‚·‚éB
		lifetime -= delta;

		if (lifetime <= 0.0f)
		{
			GetObjectLayer()->RemoveObject(this);
		}
	}
}

void Attack_Punch::SetRange(int range)
{
	switch (GetDirection())
	{
	case Direction::Back:  punchRect = Rect(getPosition().x - 1,     getPosition().y,     2,     range); break;
	case Direction::Left:  punchRect = Rect(getPosition().x - range, getPosition().y - 1, range, 2); break;
	case Direction::Front: punchRect = Rect(getPosition().x - 1,     getPosition().y - range, 2, range); break;
	case Direction::Right: punchRect = Rect(getPosition().x,         getPosition().y - 1, range, 2); break;
	}
}

void Attack_Punch::HitSparkle(const cocos2d::Vec2& position)
{
	EffectFactory::getInstance()->CreateEffect(GetObjectLayer(), EffectID::PunchHit)->setPosition(position);
}
