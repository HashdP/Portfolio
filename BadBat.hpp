#pragma once
#include "Enemy.hpp"

class BadBat : public Enemy
{
private:
	BadBat() = default;

	float searchInterval;
	int attackCount;
	float attackInterval;

public:
	virtual ~BadBat() = default;

	static BadBat* create(ObjectLayer* objectLayer, const CharaData& charaData);
	virtual bool init(ObjectLayer* objectLayer, const CharaData& charaData);
	virtual void CharacterUpdate(float delta) override;

	virtual void Damage(int damage, float freeze, const cocos2d::Vec2& knockBack) override;

	virtual void OnRemove() override;
};
