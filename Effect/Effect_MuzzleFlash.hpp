#pragma once
#include "Effect/Effect.hpp"

class Effect_MuzzleFlash : public Effect
{
public:
	CREATE_FUNC(Effect_MuzzleFlash);

	virtual bool init() override;
};
