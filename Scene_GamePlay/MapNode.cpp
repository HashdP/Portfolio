#include "Scene_GamePlay/MapNode.hpp"
#include "Scene_GamePlay/Floor.hpp"
#include "GameObject/Character/Character.hpp"
#include "myutil.hpp"
#include "Scene_GamePlay/Route.hpp"

USING_NS_CC;

MapNode::MapNode() :
	position(cocos2d::Point(0, 0)),
	bossID(CharacterID::None)
{
	for (int i = 0; i < (int)RouteDirection::RouteNum; ++i)
		route[i] = std::make_shared<Route>();
}

void MapNode::SetFloorMap(const std::array<std::array<Floor*, FIELD_Y_NUM>, FIELD_X_NUM>& floor)
{
	for(int x = 0; x < FIELD_X_NUM; ++x)
		for (int y = 0; y < FIELD_Y_NUM; ++y)
		{
			switch (floor[x][y]->GetFloorID())
			{
			case FloorID::Floor_1: floorMap[x][y] = 'f'; break;
			case FloorID::Water_1: floorMap[x][y] = 'w'; break;
			}
		}
}

const Route MapNode::GetRoute(RouteDirection routeDir) const
{
	return *route[static_cast<int>(routeDir)];
}

void MapNode::SetRouteState(RouteDirection routeDir, RouteState routeState)
{
	route[static_cast<int>(routeDir)]->SetState(routeState);
}

CharacterID MapNode::GetThisStageRandomEnemy() const
{
	std::vector<CharacterID> stageEnemy;
	switch (routeLevel)
	{
	case 1:
		stageEnemy.emplace_back(CharacterID::PunchBound);
		break;

	case 2:
		stageEnemy.emplace_back(CharacterID::PunchBound);
		stageEnemy.emplace_back(CharacterID::ShotBound);
		stageEnemy.emplace_back(CharacterID::MagicBound);
		break;

	case 3:
		stageEnemy.emplace_back(CharacterID::PunchBound);
		stageEnemy.emplace_back(CharacterID::PunchBound2);
		stageEnemy.emplace_back(CharacterID::ShotBound);
		stageEnemy.emplace_back(CharacterID::ShotBound2);
		stageEnemy.emplace_back(CharacterID::MagicBound);
		stageEnemy.emplace_back(CharacterID::MagicBound2);
		stageEnemy.emplace_back(CharacterID::BadBat);
		break;

	case 4:
		stageEnemy.emplace_back(CharacterID::PunchBound2);
		stageEnemy.emplace_back(CharacterID::PunchBound3);
		stageEnemy.emplace_back(CharacterID::ShotBound2);
		stageEnemy.emplace_back(CharacterID::ShotBound3);
		stageEnemy.emplace_back(CharacterID::MagicBound2);
		stageEnemy.emplace_back(CharacterID::MagicBound3);
		stageEnemy.emplace_back(CharacterID::BadBat);
		break;
	}

	float rand = random<float>(0.0f, 1.0f);

	for (unsigned i = 0; i < stageEnemy.size(); ++i)
	{
		if (rand < 1.0f / stageEnemy.size() * (i + 1))
		{
			return stageEnemy[i];
		}
	}

	return CharacterID::ShotBound;
}
