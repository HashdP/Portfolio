#pragma once
#include "Scene_GamePlay/Layer/OtherWindowLayer/OtherWindowLayer.hpp"

class TutorialLayer : public OtherWindowLayer
{
private:
	TutorialLayer() = default;

public:
	CREATE_FUNC(TutorialLayer);
	bool init() override;
};
