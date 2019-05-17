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
	//���
	RouteState state;

	//�ړI�n
	std::array<std::shared_ptr<MapNode>, 2> destination;

public:
	Route();
	Route(RouteState state, std::shared_ptr<MapNode> destination1, std::shared_ptr<MapNode> destination2);

	inline RouteState GetState() const { return state; }
	inline void SetState(RouteState state) { this->state = state; }

	//�ړI�n��Ԃ�
	inline std::shared_ptr<MapNode> GetDestination(std::shared_ptr<MapNode> start) const { return start == destination[0] ? destination[1] : destination[0]; }
};
