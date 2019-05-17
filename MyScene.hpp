#pragma once
#include "cocos2d.h"

/*
ƒVƒ“ƒOƒ‹ƒgƒ“‚ÌInputManager‚ÌUpdateŠÖ”‚ğ‚Ü‚í‚·
*/

class MyScene : public cocos2d::Scene
{
public:
	virtual ~MyScene() = default;

	CREATE_FUNC(MyScene);

	virtual bool init() override;
	virtual void update(float delta) override;
};
