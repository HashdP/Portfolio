#include "FieldObject.hpp"

USING_NS_CC;

bool FieldObject::init(ObjectLayer* objectLayer)
{
	if (!GameObject::init(objectLayer))
		return false;

	return true;
}

