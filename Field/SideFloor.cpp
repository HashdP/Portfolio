#include "SideFloor.hpp"
#include "MapNode.hpp"
#include "Route.hpp"
#include "Floor.hpp"

SideFloor* SideFloor::create(MapNode& mapNode)
{
	SideFloor* pRet = new(std::nothrow) SideFloor();
	if (pRet && pRet->init(mapNode))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool SideFloor::init(MapNode& mapNode)
{
	if (!Node::init())
		return false;

	for (int side = 0; side < 2; ++side)
	{
		for (int y = 0; y < 5; ++y)
		{
			RouteState state = RouteState::None;
			if (y == 0 || y == FIELD_Y_NUM - 1)
				state = mapNode.GetRoute(static_cast<RouteDirection>(side * 2 + 1 - y / (FIELD_Y_NUM - 1))).GetState();

			for (int x = 0; x < 5; ++x)
			{
				Floor* floor = nullptr;

				if (state == RouteState::None)
					floor = Floor::create(false, "Images/Field/water_1.png", 0.5f, 0.9f);
				else
					floor = Floor::create(false, "Images/Field/floor_1.png", 0.5f, 2.0f / 3.0f);

				floor->setPosition(-464 + 800 * side + FIELD_SQUARE_SIZE * x, -64 + FIELD_SQUARE_SIZE * y);

				this->addChild(floor, 4 - y);
			}
		}
	}

	return true;
}
