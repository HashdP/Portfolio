#include "Ending1.hpp"
#include "Supervisor.hpp"

USING_NS_CC;

bool Ending1::init()
{
	Sprite* bg = Sprite::create("Images/Ending/ending_background_1.png");
	this->addChild(bg);

	for (int x = 0; x < 3; ++x)
	{
		Supervisor* supervisor = Supervisor::create(nullptr);
		supervisor->SetDirection(Direction::Front);
		supervisor->unscheduleUpdate();
		supervisor->setPosition(-40 + 40 * x, 10);
		this->addChild(supervisor);
	}

	for (int y = 0; y < 2; ++y)
	{
		Supervisor* supervisor_r = Supervisor::create(nullptr);
		supervisor_r->SetDirection(Direction::Right);
		supervisor_r->SetState("Stay");
		supervisor_r->unscheduleUpdate();
		supervisor_r->setPosition(-50, -20 - y * 40);
		this->addChild(supervisor_r);

		Supervisor* supervisor_l = Supervisor::create(nullptr);
		supervisor_l->SetDirection(Direction::Left);
		supervisor_l->SetState("Stay");
		supervisor_l->unscheduleUpdate();
		supervisor_l->setPosition(50, -20 - y * 40);
		this->addChild(supervisor_l);
	}

	return true;
}