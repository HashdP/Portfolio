#pragma once
#include "Effect.hpp"

class Effect_BulletHit : public Effect
{
public:
	CREATE_FUNC(Effect_BulletHit);

	virtual bool init() override;
};