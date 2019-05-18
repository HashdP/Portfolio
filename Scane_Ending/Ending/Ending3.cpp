#include "Scane_Ending/Ending/Ending3.hpp"

USING_NS_CC;

bool Ending3::init()
{
	Sprite* bg = Sprite::create("Images/Ending/ending_background_3.png");
	this->addChild(bg);

	return true;
}