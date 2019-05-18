#pragma once
#include "Wall.hpp"

class Wall_Boss : public Wall
{
private:
	Wall_Boss() = default;

public:
	virtual ~Wall_Boss() = default;

	CREATE_FUNC(Wall_Boss);

	virtual bool init();
};
