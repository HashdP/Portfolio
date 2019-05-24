#pragma once
#include "Character.hpp"

class Supervisor : public Character
{
private:
	Supervisor() = default;

public:
	virtual ~Supervisor() = default;

	CREATE_FUNC_WITH_OBJECTLAYER(Supervisor);
	virtual bool init(ObjectLayer* objectLayer);
};
