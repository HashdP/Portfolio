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

	//�ړ��\�ȃu���b�N
	bool moveable;

	//�o�H�T���p�̕ϐ�
	std::vector<Floor*> nextFloor;
	int cost;
	bool done;
	Floor* fromFloor;

	//�t���A�C�x���g
	std::string hukidashiText;
	std::function<bool(void)> floorEvent = nullptr;
	std::function<bool(void)> isEventActiveFunc = nullptr;

public:
	//�ꖇ�摜�̂݁i�ǁA���j�̃I�u�W�F�N�g
	static Floor* create(bool moveable, std::string file, float anchorX = 0.5f, float anchorY = 0.5f);

	virtual bool init(bool moveable, std::string file, float anchorX, float anchorY);

	inline FloorID GetFloorID() { return floorID; }
	inline void SetFloorID(FloorID floorID) { this->floorID = floorID; }

	inline bool GetMoveable() { return moveable; }

	//�ׂ̏�
	inline const std::vector<Floor*>& GetNextFloor() { return nextFloor; }
	void SetNextFloor(Floor* floor, Direction dir);

	//�d��
	inline int GetCost() { return cost; }
	inline void SetCost(int cost) { this->cost = cost; }

	//�T���ς݂��ǂ���
	inline bool GetDone() { return done; }
	inline void SetDone(bool done) { this->done = done; }

	//�ǂ����痈����
	inline Floor* GetFromFloor() { return fromFloor; }
	inline void SetFromFloor(Floor* floor) { this->fromFloor = floor; }

	//�o�H�T���p�̕ϐ�������������
	void ClearRouteSeachValue();

	//�t���A�C�x���g
	inline std::string GetHukidashiText() { return hukidashiText; }
	inline bool FloorEvent() { if (floorEvent != nullptr) return floorEvent(); return false; }
	inline bool IsFloorEventActive() { if (isEventActiveFunc == nullptr) return true; return isEventActiveFunc(); }
	void SetFloorEvent(std::string text, std::function<bool(void)> floorEventFunc = nullptr, std::function<bool(void)> isEventActive = nullptr);
	inline void ClearFloorEvent() { hukidashiText = ""; floorEvent = nullptr; }
};
