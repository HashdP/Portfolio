#include "Attack.hpp"
#include "ObjectLayer.hpp"
#include "Player.hpp"
#include "SoundManager.hpp"

USING_NS_CC;

bool Attack::init(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float dmg, float frz, float kb, SoundID hitSEID)
{
	if (!GameObject::init(objectLayer))
		return false;

	setPosition(originPosition);
	SetDirection(direction);

	this->parentID = parentID;
	this->damage = dmg;
	this->freeze = frz;
	this->knockback = kb;
	this->hitSoundID = hitSEID;

	return true;
}

bool Attack::HitAttack(std::function<bool(const Character&)> func)
{
	Vec2 hitPos = GetObjectLayer()->DamageCharacter(parentID, damage, freeze, knockback, GetDirection(), func);

	if (hitPos != Vec2(10000, 10000))
	{
		//ヒットエフェクト
		HitSparkle(hitPos);
		SoundManager::getInstance()->Play2DSound(hitSoundID);

		GetObjectLayer()->RemoveObject(this);
		return true;
	}

	return false;
}

bool Attack::HitAttack(const Rect& attackRect)
{
	return HitAttack([&attackRect](const Character& chara)->bool { return chara.GetHitBox().intersectsRect(attackRect); });
}

bool Attack::HitAttack(Vec2& attackPosition, float radius)
{
	return HitAttack([&attackPosition, radius](const Character& chara)->bool { return chara.getPosition().getDistance(attackPosition) <= radius; });
}