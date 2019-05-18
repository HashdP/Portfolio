#pragma once
#include "Wall.hpp"

class Wall_Route : public Wall
{
private:
	Wall_Route() = default;

public:
	virtual ~Wall_Route() = default;

	CREATE_FUNC(Wall_Route);

	virtual bool init();
};

