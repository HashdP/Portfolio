#pragma once
#include "cocos2d.h"

/*
�V���O���g����InputManager��Update�֐����܂킷
*/

class MyScene : public cocos2d::Scene
{
public:
	virtual ~MyScene() = default;

	CREATE_FUNC(MyScene);

	virtual bool init() override;
	virtual void update(float delta) override;
};
