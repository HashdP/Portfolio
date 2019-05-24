#include "Light.hpp"

USING_NS_CC;

Light* Light::create(std::string overlayFile, int strength)
{
	Light* obj = new(std::nothrow) Light();

	if (obj && obj->init(overlayFile, strength))
	{
		obj->autorelease();
		return obj;
	}
	else
	{
		delete obj;
		obj = nullptr;
		return nullptr;
	}
}

bool Light::init(std::string overlayFile, int strength)
{
	if (!Node::init())
		return false;

	//オーバーレイ
	Texture2D* lightmaskTexture = Director::getInstance()->getTextureCache()->addImage(overlayFile);

	for (int i = 0; i < strength; ++i)
	{
		Sprite* sprite = Sprite::createWithTexture(lightmaskTexture);
		sprite->setBlendFunc({ GL_DST_COLOR , GL_ONE });
		this->addChild(sprite);
	}

	this->setCascadeColorEnabled(true);
	this->setCascadeOpacityEnabled(true);

	return true;
}
