#include "HPGauge.hpp"
#include "Character.hpp"

USING_NS_CC;

float HPGauge::baseScale = 0.9f;

bool HPGauge::init()
{
	if (!Node::init())
		return false;

	scheduleUpdate();

	Texture2D* gaugeBase_tex = Director::getInstance()->getTextureCache()->addImage("Images/GUI/wafuu_mini1.png");
	Sprite* gaugeBase = Sprite::createWithTexture(gaugeBase_tex);
	this->addChild(gaugeBase);

	Texture2D* gauge_tex = Director::getInstance()->getTextureCache()->addImage("Images/GUI/button_blue_2.png");
	gauge = Sprite::createWithTexture(gauge_tex);
	gauge->setAnchorPoint(Vec2(1.0f * 18 / 267, 0.5f));
	gauge->setPositionX(-gauge_tex->getContentSize().width / 2 + 32);
	gauge->setScaleX(baseScale);
	this->addChild(gauge);

	this->setScaleY(0.05f);
	this->setScaleX(0.2f);

	return true;
}

void HPGauge::update(float delta)
{
	visibleTime -= delta;

	if (visibleTime <= 0.0f)
	{
		visibleTime = 0.0f;
		this->setVisible(false);
	}
}

void HPGauge::SetHP(int hp, int maxHP)
{
	float mul = 1.0f * hp / maxHP;
	if (1.0f < mul)
		mul = 1.0f;
	else if (mul < 0.0f)
		mul = 0.0f;

	gauge->setScaleX(baseScale * mul);
}
