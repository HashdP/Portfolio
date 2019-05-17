#pragma once
#include "cocos2d.h"

enum Direction;

enum FloorID
{
	Floor_1,
	Water_1
};

class Floor : public cocos2d::Node
{
private:
	Floor() = default;

	FloorID floorID;

	//移動可能なブロック
	bool moveable;

	//経路探索用の変数
	std::vector<Floor*> nextFloor;
	int cost;
	bool done;
	Floor* fromFloor;

	//フロアイベント
	std::string hukidashiText;
	std::function<bool(void)> floorEvent = nullptr;
	std::function<bool(void)> isEventActiveFunc = nullptr;

public:
	//一枚画像のみ（壁、床）のオブジェクト
	static Floor* create(bool moveable, std::string file, float anchorX = 0.5f, float anchorY = 0.5f);

	virtual bool init(bool moveable, std::string file, float anchorX, float anchorY);

	inline FloorID GetFloorID() { return floorID; }
	inline void SetFloorID(FloorID floorID) { this->floorID = floorID; }

	inline bool GetMoveable() { return moveable; }

	//隣の床
	inline const std::vector<Floor*>& GetNextFloor() { return nextFloor; }
	void SetNextFloor(Floor* floor, Direction dir);

	//重み
	inline int GetCost() { return cost; }
	inline void SetCost(int cost) { this->cost = cost; }

	//探索済みかどうか
	inline bool GetDone() { return done; }
	inline void SetDone(bool done) { this->done = done; }

	//どこから来たか
	inline Floor* GetFromFloor() { return fromFloor; }
	inline void SetFromFloor(Floor* floor) { this->fromFloor = floor; }

	//経路探索用の変数を初期化する
	void ClearRouteSeachValue();

	//フロアイベント
	inline std::string GetHukidashiText() { return hukidashiText; }
	inline bool FloorEvent() { if (floorEvent != nullptr) return floorEvent(); return false; }
	inline bool IsFloorEventActive() { if (isEventActiveFunc == nullptr) return true; return isEventActiveFunc(); }
	void SetFloorEvent(std::string text, std::function<bool(void)> floorEventFunc = nullptr, std::function<bool(void)> isEventActive = nullptr);
	inline void ClearFloorEvent() { hukidashiText = ""; floorEvent = nullptr; }
};
