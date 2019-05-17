#pragma once
#include "GameObject.hpp"

enum Direction
{
	Back,
	Left,
	Front,
	Right,

	Num
};

class FieldObject : public GameObject
{
private:
	Direction direction;

protected:
	FieldObject() = default;

public:
	virtual ~FieldObject() = default;

	virtual bool init(ObjectLayer* objectLayer) override;

	inline void SetDirection(Direction direction) { this->direction = direction; }
	inline Direction GetDirection() const { return direction; }
};