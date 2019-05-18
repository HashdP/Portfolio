#pragma once
#include "Field/Area/Area.hpp"

class Area_Route : public Area
{
public:
	Area_Route() = default;

public:
	virtual ~Area_Route() = default;

	static Area_Route* create(MapNode& mapNode);
	bool init(MapNode& mapNode) override;

	virtual void InitializeFloor() override;
};

