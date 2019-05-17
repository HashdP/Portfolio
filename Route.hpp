#pragma once
#include "cocos2d.h"
#include "CocosRef.hpp"
#include <array>

class MapNode;

enum class RouteState
{
	None,
	Normal,
	Dirt,
	HeavyDirt,
	BadDirt,
	DangerousDirt
};

class Route
{
private:
	//状態
	RouteState state;

	//目的地
	std::array<std::shared_ptr<MapNode>, 2> destination;

public:
	Route();
	Route(RouteState state, std::shared_ptr<MapNode> destination1, std::shared_ptr<MapNode> destination2);

	inline RouteState GetState() const { return state; }
	inline void SetState(RouteState state) { this->state = state; }

	//目的地を返す
	inline std::shared_ptr<MapNode> GetDestination(std::shared_ptr<MapNode> start) const { return start == destination[0] ? destination[1] : destination[0]; }
};
