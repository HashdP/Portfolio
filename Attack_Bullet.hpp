#pragma once
#include "Attack.hpp"

class Attack_Bullet : public Attack
{
private:
	Attack_Bullet() = default;

	float bulletLength;
	cocos2d::Rect bulletRect;
	bool basehit;

	virtual void HitSparkle(const cocos2d::Vec2& position) override;

public:
	virtual ~Attack_Bullet() = default;
	ATTACK_CREATE_FUNC(Attack_Bullet);
	virtual bool init(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float dmg, float frz, float kb, SoundID hitSEID) override;
	virtual void update(float delta) override;
};
