#pragma once
#include "Enemy.hpp"

class Bound : public Enemy
{
private:
	//ƒŒƒxƒ‹
	int level;

public:
	virtual ~Bound() = default;

	virtual bool init(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData, float searchInterval, float nextRootRange);

	inline int GetLevel() { return level; }
};
