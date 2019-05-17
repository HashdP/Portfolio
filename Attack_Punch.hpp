#pragma once
#include "Attack.hpp"

class Attack_Punch : public Attack
{
private:
	Attack_Punch() = default;

	cocos2d::Rect punchRect;

	float lifetime;

	virtual void HitSparkle(const cocos2d::Vec2& position) override;

public:
	virtual ~Attack_Punch() = default;
	ATTACK_CREATE_FUNC(Attack_Punch);
	virtual bool init(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float dmg, float frz, float kb, SoundID hitSEID) override;
	virtual void update(float delta) override;

	void SetRange(int range);
};
