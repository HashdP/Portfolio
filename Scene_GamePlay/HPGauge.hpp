#pragma once
#include "cocos2d.h"

class Character;

class HPGauge : public cocos2d::Node
{
private:
	HPGauge() = default;

	static float baseScale;

	cocos2d::Sprite* gauge;

	float visibleTime = 0.0f;

public:
	virtual ~HPGauge() = default;
	CREATE_FUNC(HPGauge);
	virtual bool init() override;
	virtual void update(float delta) override;

	/*
	time�b��HP�Q�[�W��\������
	*/
	inline void Visible(float time) { visibleTime = time; this->setVisible(true); }

	/*
	HP��ύX����
	*/
	void SetHP(int hp, int maxHP);
};
