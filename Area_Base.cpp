#pragma execution_character_set("utf-8")

#include "Area_Base.hpp"
#include "Floor.hpp"
#include "WorkshopLayer.hpp"

USING_NS_CC;

Area_Base* Area_Base::create(MapNode& mapNode, ObjectLayer* objectLayer, WorkshopLayer* workshop)
{
	Area_Base* pRet = new(std::nothrow) Area_Base();
	if (pRet && pRet->init(mapNode, objectLayer, workshop))
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

bool Area_Base::init(MapNode& mapNode, ObjectLayer* objectLayer, WorkshopLayer* workshop)
{
	if (!Area::init(mapNode))
		return false;

	this->objectLayer = objectLayer;
	this->workshopLayer = workshop;

	return true;
}

void Area_Base::InitializeFloor()
{
	for (int x = 0; x < FIELD_X_NUM; ++x)
	{
		CreateFloor(FloorID::Floor_1, x, 0);
		CreateFloor(FloorID::Floor_1, x, FIELD_Y_NUM - 1);

		if (5 <= x && x < 15)
		{
			CreateFloor(FloorID::Floor_1, x, 1);
			CreateFloor(FloorID::Floor_1, x, 2);
			CreateFloor(FloorID::Floor_1, x, 3);
		}
		else
		{
			CreateFloor(FloorID::Water_1, x, 1);
			CreateFloor(FloorID::Water_1, x, 2);
			CreateFloor(FloorID::Water_1, x, 3);
		}
	}
}

void Area_Base::CreateFloorEvent()
{
	auto visibleWorkshop = [this]()
	{
		workshopLayer->ChangeVisible(*objectLayer);
		return true;
	};

	GetFloor(7, 4)->SetFloorEvent("「工房を使用する」", visibleWorkshop);
	GetFloor(8, 4)->SetFloorEvent("「工房を使用する」", visibleWorkshop);
}
