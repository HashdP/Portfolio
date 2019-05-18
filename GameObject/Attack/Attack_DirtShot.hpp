#pragma once
#include "GameObject/Attack/Attack.hpp"

class Attack_DirtShot : public Attack
{
private:
	Attack_DirtShot() = default;

	cocos2d::Vec2 dirVec;
	cocos2d::ParticleSystemQuad* particle;
	float deleteInterval;
	float speed;

	virtual void HitSparkle(const cocos2d::Vec2& position) override;

public:
	virtual ~Attack_DirtShot() = default;
	static Attack_DirtShot* create(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float speed, float dmg, float frz, float kb, SoundID hitSEID);
	virtual bool init(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float speed, float dmg, float frz, float kb, SoundID hitSEID);
	virtual void update(float delta) override;

	virtual float GetFieldPositionY();
};
