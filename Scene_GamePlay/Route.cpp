#include "Scene_GamePlay/Route.hpp"
#include "CocosRef.hpp"

USING_NS_CC;

Route::Route() : 
	state(RouteState::None)
{
}

Route::Route(RouteState state, std::shared_ptr<MapNode> destination1, std::shared_ptr<MapNode> destination2) : 
	state(state), 
	destination({ destination1, destination2 })
{
}
