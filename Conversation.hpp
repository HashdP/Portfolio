#pragma once
#include "cocos2d.h"
#include <fstream>

class Character;

class Conversation : public cocos2d::Node
{
private:
	float time = 1000.0f;
	float interval;

	std::ifstream ifs;

	Character* chara;

public:
	static Conversation* create(std::string file, Character* chara, float interval = 5.0f);

	virtual bool init(std::string file, Character* chara, float interval);
	virtual void update(float delta) override;
};

