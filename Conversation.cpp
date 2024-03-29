#include "Conversation.hpp"
#include "Character.hpp"

USING_NS_CC;

Conversation* Conversation::create(std::string file, Character* chara, float interval)
{
	Conversation* conv = new(std::nothrow) Conversation();

	if (conv && conv->init(file, chara, interval))
	{
		conv->autorelease();
		return conv;
	}
	else
	{
		delete conv;
		conv = nullptr;
		return nullptr;
	}
}

bool Conversation::init(std::string file, Character* chara, float interval)
{
	if (!Node::init())
		return false;

	scheduleUpdate();

	ifs.open(file);
	//切り捨て
	std::string str;
	std::getline(ifs, str);

	this->chara = chara;
	this->interval = interval;

	return true;
}

void Conversation::update(float delta)
{
	time += delta;

	if (interval < time)
	{
		std::string str;

		if (std::getline(ifs, str))
		{
			chara->Speak(str, interval);

			time = 0.0f;
		}
		else
		{
			//全て話したら終了する
			removeFromParent();
		}
	}
}