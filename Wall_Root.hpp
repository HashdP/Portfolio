#pragma once
#include "Wall.hpp"

class Wall_Root : public Wall
{
private:
	Wall_Root() = default;

public:
	virtual ~Wall_Root() = default;

	CREATE_FUNC(Wall_Root);

	virtual bool init();
};

