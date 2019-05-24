#pragma once
#include "Bound.hpp"

class MagicBound : public Bound
{
private:
	MagicBound() = default;

	float deltatime;
	float attackspan;

	int atkJumpCounter;

public:
	virtual ~MagicBound() = default;
	static MagicBound* create(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData);
	virtual bool init(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData);

	virtual void CharacterUpdate(float delta) override;

	virtual void OnRemove() override;
};
