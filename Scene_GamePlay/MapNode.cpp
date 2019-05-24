#include "MapNode.hpp"
#include "Floor.hpp"
#include "Route.hpp"
#include "Character.hpp"
#include "myutil.hpp"
#include <sqlite3.h>

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

	sqlite3* sql3;
	if (sqlite3_open("Data/StageData.db", &sql3) == SQLITE_OK)
	{
		sqlite3_stmt* statement;
		std::string cmd = "select * from Level" + std::to_string(routeLevel);
		const char* command = cmd.c_str();

		sqlite3_prepare_v2(sql3, command, -1, &statement, &command);
		while (sqlite3_step(statement) == SQLITE_ROW)
		{
			std::string enemyIDStr = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
			stageEnemy.emplace_back(CharacterID_Util::ToEnum(enemyIDStr));
		}

		sqlite3_finalize(statement);
		sqlite3_close(sql3);
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
