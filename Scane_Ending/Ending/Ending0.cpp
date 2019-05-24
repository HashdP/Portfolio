#include "Ending0.hpp"
#include "PunchBound.hpp"
#include "CharaData.hpp"

USING_NS_CC;

bool Ending0::init()
{
	Sprite* bg = Sprite::create("Images/Ending/ending_background_0.png");
	this->addChild(bg);

	PunchBound* bound1 = PunchBound::create(nullptr, CharacterID::PunchBound, 1, CharaData("PunchBound"));
	bound1->unscheduleUpdate();
	bound1->setPosition(-60, 0);
	bound1->SetState("Move");
	this->addChild(bound1);

	PunchBound* bound2 = PunchBound::create(nullptr, CharacterID::PunchBound, 1, CharaData("PunchBound"));
	bound2->unscheduleUpdate();
	bound2->setPosition(50, -50);
	this->addChild(bound2);

	return true;
}