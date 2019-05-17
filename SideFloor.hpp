#pragma once
#include "cocos2d.h"

class MapNode;

class SideFloor : public cocos2d::Node
{
private:
	SideFloor() = default;

public:
	static SideFloor* create(MapNode& mapNode);
	bool init(MapNode& mapNode);
};

