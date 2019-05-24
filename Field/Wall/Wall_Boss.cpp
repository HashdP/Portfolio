#include "Wall_Boss.hpp"

USING_NS_CC;

bool Wall_Boss::init()
{
	if (!Wall::init())
		return false;

	//•Ç
	Texture2D* texWall = Director::getInstance()->getTextureCache()->addImage("Images/Field/bg_wall.png");
	Texture2D* texLadder = Director::getInstance()->getTextureCache()->addImage("Images/Field/bg_wall_ladder.png");
	for (int i = 0; i < 11; ++i)
	{
		Sprite* bg = nullptr;
		if (i == 5)
			bg = Sprite::createWithTexture(texLadder);
		else
			bg = Sprite::createWithTexture(texWall);

		bg->setPosition(-464 + i * 96, 100);

		this->addChild(bg, -1);
	}

	//ƒ‰ƒCƒg
	for (int i = 0; i < 4; ++i)
	{
		CreateLight(Vec2(-270 + 180 * i, 123));
	}

	return true;
}
