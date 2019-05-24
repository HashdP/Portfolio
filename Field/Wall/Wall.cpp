#include "Wall.hpp"
#include "Light.hpp"
#include "myutil.hpp"

USING_NS_CC;

bool Wall::init()
{
	if (!Node::init())
		return false;

	return true;
}

void Wall::CreateLight(const Vec2& position)
{
	Texture2D* tex = Director::getInstance()->getTextureCache()->addImage("Images/Field/light.png");
	Sprite* lightBase = Sprite::createWithTexture(tex);
	lightBase->setPosition(position);
	this->addChild(lightBase);

	Light* wallLight = Light::create("Images/lightmask.png", 3);
	wallLight->setPosition(position + Vec2(0, -23));
	wallLight->setColor(Color3B(255, 160, 80));
	wallLight->setScale(0.8f);
	myutil::SetGlobalZOrderRecursive(wallLight, 1);
	this->addChild(wallLight, INT_MAX);
}
