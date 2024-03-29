#include "MoveAI.hpp"
#include "Area.hpp"
#include "Floor.hpp"
#include "GameObject.hpp"
#include "myutil.hpp"

USING_NS_CC;

void MoveAI::SetData(float searchInterval, float nextRootRange)
{
	this->searchInterval = searchInterval;
	this->nextRootRange = nextRootRange;
}

bool MoveAI::SearchRoute(const Area& area, const Vec2& fromPos, const Vec2& targetPos, bool moveableWater)
{
	std::vector<Floor*>& floors = area.GetFloorVector();
	for (Floor* floor : floors)
		floor->ClearRouteSeachValue();

	Floor* startFloor = area.GetFloor(fromPos);

	//初めのFloorのコストを０にする
	startFloor->SetCost(0);

	while (true)
	{
		Floor* toFloor = nullptr;

		for (Floor* floor : floors)
		{
			//まだ探索していない／探索済みの場合
			if (floor->GetDone() || floor->GetCost() < 0)
				continue;

			//processFloorが未設定の場合
			if (!toFloor)
			{
				toFloor = floor;
				continue;
			}

			if (floor->GetCost() < toFloor->GetCost())
				toFloor = floor;
		}

		//すべて探索し終わったら探索を終了する
		if (!toFloor)
			break;

		toFloor->SetDone(true);

		for (Floor* floor : toFloor->GetNextFloor())
		{
			//隣に移動可能なFloorがある場合
			if (floor != nullptr && (floor->GetMoveable() || moveableWater))
			{
				//必要なコスト（エッジの重みはすべて１）
				int needCost = toFloor->GetCost() + 1;

				if (floor->GetCost() < 0 || needCost < floor->GetCost())
				{
					floor->SetCost(needCost);
					floor->SetFromFloor(toFloor);
				}
			}
		}
	}

	//routeに設定する
	route.clear();

	Floor* reserveFloor = area.GetFloor(targetPos);
	while (true)
	{
		if (reserveFloor == startFloor)
			break;
		if (reserveFloor == nullptr)
		{
			//探索失敗
			return false;
		}

		route.push_back(reserveFloor);

		reserveFloor = reserveFloor->GetFromFloor();
	}

	return true;
}

Vec2 MoveAI::MoveUpdate(float delta, const Area& area, const Vec2& fromPos, const Vec2& targetPos, bool moveableWater, bool zeroUpdate)
{
	interval += delta;

	if (searchInterval < interval || (zeroUpdate && route.size() == 0))
	{
		SearchRoute(area, fromPos, targetPos, moveableWater);
		interval = 0.0f;
	}

	Vec2 dirVec = Vec2::ZERO;

	if (route.size() != 0)
	{
		Floor* toFloor = *(route.end() - 1);
		dirVec = myutil::GetDirectionVec8(fromPos, toFloor->getPosition());

		if (fromPos.distance(toFloor->getPosition()) < nextRootRange)
			route.pop_back();
	}
	else
	{
		dirVec = myutil::GetDirectionVec8(fromPos, targetPos);
	}

	return dirVec;
}
