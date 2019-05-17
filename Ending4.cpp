#include "Ending4.hpp"

USING_NS_CC;

bool Ending4::init()
{
	Sprite* bg2 = Sprite::create("Images/Ending/ending_background_4_2.png");
	this->addChild(bg2);

	Sprite* bg3 = Sprite::create("Images/Ending/ending_background_4_3.png");
	bg3->runAction(
		RepeatForever::create(
			Sequence::create(
				EaseInOut::create(MoveBy::create(1.0f, Vec2(0, -5)), 2),
				EaseInOut::create(MoveBy::create(1.0f, Vec2(0, 5)), 2),
				NULL
			)
		)
	);
	this->addChild(bg3);

	Sprite* bg1 = Sprite::create("Images/Ending/ending_background_4_1.png");
	this->addChild(bg1);

	return true;
}