#include "Floor.hpp"
#include "GameObject.hpp"

USING_NS_CC;

Floor* Floor::create(bool moveable, std::string file, float anchorX, float anchorY)
{
	Floor* floor = new(std::nothrow) Floor();

	if (floor && floor->init(moveable, file, anchorX, anchorY))
	{
		floor->autorelease();
		return floor;
	}
	else
	{
		delete floor;
		floor = nullptr;
		return nullptr;
	}
}

bool Floor::init(bool moveable, std::string file, float anchorX, float anchorY)
{
	if (!Node::init())
		return false;

	Texture2D* tex = Director::getInstance()->getTextureCache()->addImage(file);
	Sprite* sprite = Sprite::createWithTexture(tex);
	sprite->setAnchorPoint(Vec2(anchorX, anchorY));
	this->addChild(sprite);

	this->moveable = moveable;
	
	//DirectionÇÃêîÇæÇØpush_backÇ∑ÇÈ
	for (int i = 0; i < (int)Direction::Num; ++i)
		nextFloor.push_back(nullptr);

	return true;
}

void Floor::SetNextFloor(Floor* floor, Direction dir)
{
	nextFloor[(int)dir] = floor;

	if (floor != nullptr)
	{
		switch (dir)
		{
		case Direction::Front: floor->nextFloor[(int)Direction::Back] = this; break;
		case Direction::Back: floor->nextFloor[(int)Direction::Front] = this; break;
		case Direction::Left: floor->nextFloor[(int)Direction::Right] = this; break;
		case Direction::Right: floor->nextFloor[(int)Direction::Left] = this; break;
		}
	}
}

void Floor::ClearRouteSeachValue()
{
	cost = -1;
	done = false;
	fromFloor = nullptr;
}

void Floor::SetFloorEvent(std::string text, std::function<bool(void)> floorEventFunc, std::function<bool(void)> isEventActive)
{
	hukidashiText = text;
	floorEvent = floorEventFunc; 
	isEventActiveFunc = isEventActive;
}