#pragma once
#include "OtherWindowLayer.hpp"

class TutorialLayer : public OtherWindowLayer
{
private:
	TutorialLayer() = default;

public:
	CREATE_FUNC(TutorialLayer);
	bool init() override;
};
