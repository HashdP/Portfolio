#include "Effect/Effect.hpp"

bool Effect::init(float duration)
{
	if (!Node::init())
		return false;

	scheduleUpdate();

	deltaTime = 0.0f;
	this->duration = duration;

	return true;
}

void Effect::update(float delta)
{
	deltaTime += delta;

	if (duration <= deltaTime)
	{
		this->removeFromParent();
	}
}
