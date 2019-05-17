#pragma once
#include "cocos2d.h"
#include <array>

//�t�B�[���h��X�}�X�̐�
static const int FIELD_X_NUM = 20;
//�t�B�[���h��y�}�X�̐�
static const int FIELD_Y_NUM = 5;
//�t�B�[���h�̃}�X�̃T�C�Y
static const int FIELD_SQUARE_SIZE = 32;

enum FloorID;
enum RouteDirection;
class Floor;
class MapNode;

class Area : public cocos2d::Node
{
private:
	std::array<std::array<Floor*, FIELD_Y_NUM>, FIELD_X_NUM> floors;

protected:
	Area() = default;

	Floor* CreateFloor(FloorID floorID, int x, int y);

public:
	virtual ~Area() = default;

	virtual bool init(MapNode& MapNode);

	virtual void InitializeFloor() = 0;
	virtual void CreateFloorEvent() {}

	//Floor���擾
	Floor* GetFloor(const cocos2d::Vec2& position) const;
	inline Floor* GetFloor(int x, int y) const { return floors[x][y]; }
	std::vector<Floor*> GetFloorVector() const;

	//�ړ��\�Ń����_���ȏꏊ���擾
	const cocos2d::Vec2 GetRandomMoveablePlace(bool center = false) const;

	//�ړ��\�ȏꏊ���ǂ������擾
	bool IsMoveablePlace(const cocos2d::Vec2& position) const;
};
