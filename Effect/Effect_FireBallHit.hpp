#pragma once
#include "Effect.hpp"

class Effect_FireBallHit : public Effect
{
public:
	CREATE_FUNC(Effect_FireBallHit);

	virtual bool init() override;
};
