#include "Wall_Base.hpp"
#include "Light.hpp"

USING_NS_CC;

bool Wall_Base::init()
{
	if (!Wall::init())
		return false;

	//•Ç
	Texture2D* texWall = Director::getInstance()->getTextureCache()->addImage("Images/Field/bg_wall.png");
	Texture2D* texDoor = Director::getInstance()->getTextureCache()->addImage("Images/Field/bg_wall_door.png");
	Texture2D* texLadder = Director::getInstance()->getTextureCache()->addImage("Images/Field/bg_wall_ladder.png");

	for (int i = 0; i < 11; ++i)
	{
		Sprite* bg = nullptr;
		if (i == 4)
			bg = Sprite::createWithTexture(texDoor);
		else if (i == 6)
			bg = Sprite::createWithTexture(texLadder);
		else
			bg = Sprite::createWithTexture(texWall);
		bg->setPosition(-464 + i * 96, 100);

		this->addChild(bg, -1);
	}

	//ƒ‰ƒCƒg
	for (int i = 0; i < 5; ++i)
	{
		Vec2 position = i < 3 ? Vec2(-270 + 145 * i, 123) : Vec2(170 + 145 * (i - 3), 123);
		CreateLight(position);
	}

	return true;
}
