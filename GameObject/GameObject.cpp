#include "GameObject.hpp"

USING_NS_CC;

bool GameObject::init(ObjectLayer* objectLayer)
{
	if (!Node::init())
		return false;

	scheduleUpdate();

	this->objectLayer = objectLayer;
	onRemove = true;

	this->setCascadeColorEnabled(true);
	this->setCascadeOpacityEnabled(true);

	return true;
}
