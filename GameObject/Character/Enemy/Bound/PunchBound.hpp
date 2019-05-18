#pragma once
#include "GameObject/Character/Enemy/Bound/Bound.hpp"

class PunchBound : public Bound
{
private:
	PunchBound() = default;

public:
	virtual ~PunchBound() = default;
	static PunchBound* create(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData);
	virtual bool init(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData);

	virtual void CharacterUpdate(float delta) override;

	virtual void OnRemove() override;
};
