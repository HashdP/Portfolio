#pragma once
#include "Effect.hpp"

class Effect_PunchHit : public Effect
{
public:
	CREATE_FUNC(Effect_PunchHit);

	virtual bool init() override;
};