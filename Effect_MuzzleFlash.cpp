#include "Effect_MuzzleFlash.hpp"
#include "Light.hpp"

USING_NS_CC;

bool Effect_MuzzleFlash::init()
{
	if (!Effect::init(1.0f))
		return false;

	this->setCascadeColorEnabled(true);
	this->setCascadeOpacityEnabled(true);

	Light* light = Light::create("Images/lightmask.png", 2);
	light->setScale(2.0f);
	this->addChild(light);

	Light* light2 = Light::create("Images/lightmask.png", 2);
	light2->setScale(0.3f);
	this->addChild(light2);

	this->setColor(Color3B(255, 200, 100));
	this->runAction(FadeOut::create(0.1f));

	return true;
}
