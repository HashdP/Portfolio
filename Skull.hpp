#pragma once
#include "Character.hpp"

class Skull : public Character
{
private:
	Skull() = default;

public:
	virtual ~Skull() = default;

	CREATE_FUNC_WITH_OBJECTLAYER(Skull);
	virtual bool init(ObjectLayer* objectLayer);
};
