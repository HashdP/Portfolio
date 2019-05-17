#pragma once
#include "Area.hpp"

class ObjectLayer;
class WorkshopLayer;

class Area_Base : public Area
{
private:
	Area_Base() = default;

	ObjectLayer* objectLayer;
	WorkshopLayer* workshopLayer;

public:
	virtual ~Area_Base() = default;
	static Area_Base* create(MapNode& mapNode, ObjectLayer* objectLayer, WorkshopLayer* workshop);
	bool init(MapNode& mapNode, ObjectLayer* objectLayer, WorkshopLayer* workshop);

	void InitializeFloor() override;
	void CreateFloorEvent() override;
};

