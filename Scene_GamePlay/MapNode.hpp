#pragma once
#include "cocos2d.h"
#include "CocosRef.hpp"
#include "Field/Area/Area.hpp"
#include <array>

enum RouteDirection
{
	UpperLeft,
	LowerLeft,
	UpperRight,
	LowerRight,

	RouteNum
};

enum ItemID;
enum class CharacterID;
enum class RouteState;
class Character;
class Route;

class MapNode
{
private:
	//�}�b�v�̈ʒu���
	cocos2d::Point position;

	//���i�l�����j
	std::array<std::shared_ptr<Route>, static_cast<int>(RouteDirection::RouteNum)> route;

	//�t���A�}�b�v
	bool initFloorMap = false;
	std::array<std::array<char, FIELD_Y_NUM>, FIELD_X_NUM> floorMap;

	//���_����̋���
	int lengthFromBase = 0;

	//���[�g���x��
	int routeLevel = 0;

	//�G�l�~�[����
	int enemyNum = 0;

	CharacterID bossID;

public:
	MapNode();

	inline void SetPosition(cocos2d::Point& position) { this->position = position; }
	inline cocos2d::Point GetPosition() const { return position; }

	inline void Initielized() { initFloorMap = true; }
	inline bool IsInitFloorMap() const { return initFloorMap; }

	void SetFloorMap(const std::array<std::array<Floor*, FIELD_Y_NUM>, FIELD_X_NUM>& floor);
	inline void SetFloorMap(int x, int y, char c) { floorMap[x][y] = c; }
	inline std::array<std::array<char, FIELD_Y_NUM>, FIELD_X_NUM>& GetFloorMap() { return floorMap; }

	inline void SetLengthFromBase(int lengthFromBase) { this->lengthFromBase = lengthFromBase; }
	inline int GetLengthFromBase() const { return lengthFromBase; }

	inline void SetRouteLevel(int routeLevel) { this->routeLevel = routeLevel; }
	inline int GetRouteLevel() const { return routeLevel; }

	inline void SetEnemyNum(int enemyNum) { this->enemyNum = enemyNum; }
	inline int GetEnemyNum() const { return enemyNum; }

	inline void SetRoute(RouteDirection routeDir, std::shared_ptr<Route> route) { this->route[static_cast<int>(routeDir)] = route; }
	const Route GetRoute(RouteDirection routeDir) const;
	void SetRouteState(RouteDirection routeDir, RouteState routeState);

	inline void SetBossID(CharacterID charaID) { bossID = charaID; }
	inline CharacterID GetBossID() const { return bossID; }

	//���̃X�e�[�W�̓GID���擾����
	CharacterID GetThisStageRandomEnemy() const;
};