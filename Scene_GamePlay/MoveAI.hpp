#pragma once
#include "cocos2d.h"
#include <vector>

class Floor;
class Area;

class MoveAI
{
private:
	float interval = 10000.0f;

	//サーチ間隔
	float searchInterval;

	//目標のフロアにどれだけ近づいたら次のフロアに行くか
	float nextRootRange;

	//サーチした道順
	std::vector<Floor*> route;

public:

	/*
	探索データを設定
	searchInterval秒毎に目的地までの経路を探索する
	次のFloorにnextRouteRangeまで近づいたら次のFloorに向かう
	*/
	void SetData(float searchInterval, float nextRouteRange);

	/*
	fromPosからtargetPosまでの経路を探索しFloorに格納する
	moveableWaterがtrueなら、移動不可の場所も含めて探索する
	*/
	bool SearchRoute(const Area& area, const cocos2d::Vec2& fromPos, const cocos2d::Vec2& targetPos, bool moveableWater = false);

	/*
	関数内でSearchRouteを行い、結果から次のFloorまでの向きを取得する
	zeroUpdateをtrueにした場合、探索結果のrouteが空になったら即座に次の探索を行う。
	*/
	cocos2d::Vec2 MoveUpdate(float delta, const Area& area, const cocos2d::Vec2& fromPos, const cocos2d::Vec2& targetPos, bool moveableWater = false, bool zeroUpdate = false);

	/*
	目的地に到達して、routeが空かどうか
	*/
	inline bool IsArrive() const { return route.empty(); }
};
