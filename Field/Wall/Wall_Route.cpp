#include "Wall_Route.hpp"

USING_NS_CC;

bool Wall_Route::init()
{
	if (!Wall::init())
		return false;

	//•Ç
	Texture2D* texWall = Director::getInstance()->getTextureCache()->addImage("Images/Field/bg_wall.png");
	for (int i = 0; i < 11; ++i)
	{
		Sprite* bg = Sprite::createWithTexture(texWall);

		bg->setPosition(-464 + i * 96, 100);

		this->addChild(bg, -1);
	}

	//ƒ‰ƒCƒg
	for (int i = 0; i < 5; ++i)
	{
		CreateLight(Vec2(-270 + 180 * i, 123));
	}

	return true;
}
