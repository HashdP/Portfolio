#include "Item_Spray.hpp"
#include "ObjectLayer.hpp"
#include "Player.hpp"
#include "myutil.hpp"
#include "Route.hpp"
#include "SoundManager.hpp"

USING_NS_CC;

Item_Spray::Item_Spray(ItemID id, int num, int maxNum, RouteState effectiveState) :
	Item(id, num, maxNum),
	effectiveState(effectiveState)
{
}

bool Item_Spray::Use(int itemIndex, int weaponIndex, ItemLayer* itemLayer, ObjectLayer* objectLayer)
{
	Vec2 floorPos = myutil::ConvertPosToFloorPos(objectLayer->GetConstPlayer().getPosition());

	RouteDirection rootDir = myutil::GetRouteDirectionFromFloorPos(floorPos);

	if (rootDir != RouteDirection::RouteNum && objectLayer->GetCurrentMap().GetRoute(rootDir).GetState() == effectiveState)
	{
		SoundManager::getInstance()->Play2DSound(SoundID::UseItem_Spray);
		objectLayer->ReleaseRoot(rootDir);
		return true;
	}

	return false;
}
