#pragma once
#include "Item.hpp"
#include "MapNode.hpp"

enum RouteDirection;
enum class RouteState;

class Item_Spray : public Item
{
private:
	RouteState effectiveState;

public:
	Item_Spray(ItemID id, int num, int maxNum, RouteState effectiveState);
	virtual ~Item_Spray() = default;

	virtual bool Use(int itemIndex, int weaponIndex, ItemLayer* itemLayer, ObjectLayer* objectLayer) override;
};
