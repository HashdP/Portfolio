#pragma once
#include "GameObject/Attack/Attack.hpp"
#include "Scene_GamePlay/MoveAI.hpp"

class Attack_FireBall : public Attack
{
private:
	Attack_FireBall() = default;

	MoveAI moveAI;
	cocos2d::ParticleSystemQuad* particle;
	float interval;
	float lifetime;

	virtual void HitSparkle(const cocos2d::Vec2& position) override;

public:
	virtual ~Attack_FireBall() = default;
	ATTACK_CREATE_FUNC(Attack_FireBall);
	virtual bool init(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float dmg, float frz, float kb, SoundID hitSEID) override;
	virtual void update(float delta) override;

	virtual float GetFieldPositionY();
};
