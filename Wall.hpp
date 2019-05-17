#pragma once
#include "cocos2d.h"

class Wall : public cocos2d::Node
{
private:
	std::vector<cocos2d::Node*> overlayObjects;

protected:
	Wall() = default;

	void CreateLight(const cocos2d::Vec2& position);

public:
	virtual ~Wall() = default;

	CREATE_FUNC(Wall);

	virtual bool init();
};
