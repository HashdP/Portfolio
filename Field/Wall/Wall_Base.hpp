#pragma once
#include "Wall.hpp"

class Wall_Base : public Wall
{
private:
	Wall_Base() = default;

public:
	virtual ~Wall_Base() = default;

	CREATE_FUNC(Wall_Base);

	virtual bool init();
};

