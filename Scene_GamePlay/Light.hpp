#pragma once
#include "cocos2d.h"

class Light : public cocos2d::Node
{
private:
	Light() = default;

public:
	static Light* create(std::string overlayFile, int strength);

	virtual bool init(std::string overlayFile, int strength);
};
