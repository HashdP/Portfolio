#pragma once
#include "Area.hpp"

class Area_Root : public Area
{
public:
	Area_Root() = default;

public:
	virtual ~Area_Root() = default;

	static Area_Root* create(MapNode& mapNode);
	bool init(MapNode& mapNode) override;

	virtual void InitializeFloor() override;
};

