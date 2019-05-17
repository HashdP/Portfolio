#pragma once
#include "Area_Root.hpp"

class ObjectLayer;

class Area_Boss : public Area_Root
{
private:
	Area_Boss() = default;

	ObjectLayer* objectLayer;
	int level;

public:
	virtual ~Area_Boss() = default;

	static Area_Boss* create(MapNode& mapNode, ObjectLayer* objectLayer, int level);
	virtual bool init(MapNode& mapNode, ObjectLayer* objectLayer, int level);

	void InitializeFloor() override;
	void CreateFloorEvent() override;
};

