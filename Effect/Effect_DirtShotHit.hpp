#pragma once
#include "Effect/Effect.hpp"

class Effect_DirtShotHit : public Effect
{
public:
	CREATE_FUNC(Effect_DirtShotHit);

	virtual bool init() override;
};