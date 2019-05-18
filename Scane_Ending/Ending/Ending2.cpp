#include "Scane_Ending/Ending/Ending2.hpp"

USING_NS_CC;

bool Ending2::init()
{
	Sprite* bg = Sprite::create("Images/Ending/ending_background_2.png");
	this->addChild(bg);

	return true;
}