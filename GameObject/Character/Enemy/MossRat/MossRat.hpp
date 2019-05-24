#pragma once
#include "Enemy.hpp"

class MossRat : public Enemy
{
private:
	MossRat() = default;

	int actionType;
	float changeTypeProb;
	cocos2d::Vec2 type2ToPosition;

public:
	virtual ~MossRat() = default;

	static MossRat* create(ObjectLayer* objectLayer, const CharaData& charaData);
	virtual bool init(ObjectLayer* objectLayer, const CharaData& charaData);
	virtual void CharacterUpdate(float delta) override;

	virtual void Damage(int damage, float freeze, const cocos2d::Vec2& knockBack) override;

	void ChangeToType();
};

