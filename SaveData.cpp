#include "SaveData.hpp"
#include "Scene_GamePlay/Layer/ItemLayer.hpp"
#include "Scene_GamePlay/Layer/OtherWindowLayer/MapLayer.hpp"
#include "Item/Item.hpp"
#include "Scene_GamePlay/MapNode.hpp"
#include "Scene_GamePlay/Route.hpp"
#include "myutil.hpp"
#include <Windows.h>
#include <tchar.h>
#include <fstream>

USING_NS_CC;

SaveData* SaveData::instance = nullptr;

SaveData* SaveData::getInstance()
{
	if (instance == nullptr)
		instance = new SaveData();

	return instance;
}

SaveData::SaveData()
{
	TCHAR tPath[1024];
	GetModuleFileName(NULL, tPath, 1024);
	(*_tcsrchr(tPath, '\\')) = '\0';

#ifdef UNICODE
	char path[1024];
	wcstombs(path, tPath, 1024);
	savePath = path;
#else
	savePath = tPath;
#endif

	savePath += "\\savedata.dat";
}

void SaveData::init(ItemLayer* itemLayer, MapLayer* mapLayer)
{
	this->itemLayer = myutil::make_cocos_unique_ref<ItemLayer>(itemLayer);
	this->mapLayer = myutil::make_cocos_unique_ref<MapLayer>(mapLayer);

	if (!IsExistSaveFile())
	{
		//ファイルが存在しない場合、作成する
		std::ofstream ofs(savePath);

		SaveEndingNumber(0);
		SaveWeapon();
		SaveItem();
		SaveMap();
		SaveInterruptionPoint(Point(0, 0));
		SavePlayerPosition(myutil::ConvertFloorPosToPos(12, 3));
	}

	SaveContinueFlag(true);
}

void SaveData::finalize()
{
	itemLayer = nullptr;
	mapLayer = nullptr;
}

bool SaveData::IsExistSaveFile()
{
	std::ifstream ifs(savePath);
	return ifs.is_open();
}

bool SaveData::LoadContinueFlag()
{
	std::ifstream ifs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	if (!ifs)
		return false;

	ifs.seekg(0, std::ios_base::beg);
	return ReadData<bool>(ifs);
}

void SaveData::SaveContinueFlag(bool flag)
{
	std::ofstream ofs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	ofs.seekp(0, std::ios_base::beg);
	WriteData<bool>(ofs, flag);
}

int SaveData::LoadEndingNumber()
{
	std::ifstream ifs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	if (!ifs)
		return 0;

	ifs.seekg(sizeof(bool), std::ios_base::beg);
	return ReadData<int>(ifs);
}

void SaveData::SaveEndingNumber(int endingNumber)
{
	std::ofstream ofs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	ofs.seekp(sizeof(bool), std::ios_base::beg);
	WriteData<int>(ofs, endingNumber);
}

std::array<Item, 3> SaveData::LoadWeapon()
{
	std::ifstream ifs(savePath, std::ios::in | std::ios::binary | std::ios::ate);

	ifs.seekg(sizeof(bool) + sizeof(int), std::ios_base::beg);

	std::array<Item, 3> ret_ary;
	for (int i = 0; i < 3; ++i)
	{
		ItemID itemID = ReadData<ItemID>(ifs);
		int itemNum = ReadData<int>(ifs);

		ret_ary[i] = Item(itemID, itemNum, itemNum);
	}

	return ret_ary;
}

void SaveData::SaveWeapon()
{
	std::ofstream ofs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	ofs.seekp(sizeof(bool) + sizeof(int), std::ios_base::beg);

	for (int i = 0; i < 3; ++i)
	{
		Item weapon = itemLayer->GetWeapon(i);

		WriteData<ItemID>(ofs, weapon.GetID());
		WriteData<int>(ofs, weapon.GetNum());
	}
}

std::array<Item, 24> SaveData::LoadItem()
{
	std::ifstream ifs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	ifs.seekg(sizeof(bool) + sizeof(int) + (sizeof(ItemID) + sizeof(int)) * 3, std::ios_base::beg);

	std::array<Item, 24> ret_ary;
	for (int i = 0; i < 24; ++i)
	{
		ItemID itemID = ReadData<ItemID>(ifs);
		int itemNum = ReadData<int>(ifs);

		ret_ary[i] = Item(itemID, itemNum, itemNum);
	}

	return ret_ary;
}

void SaveData::SaveItem()
{
	std::ofstream ofs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	ofs.seekp(sizeof(bool) + sizeof(int) + (sizeof(ItemID) + sizeof(int)) * 3, std::ios_base::beg);

	for (int i = 0; i < 24; ++i)
	{
		Item item = itemLayer->GetItem(i);

		WriteData<ItemID>(ofs, item.GetID());
		WriteData<int>(ofs, item.GetNum());
	}
}

Point SaveData::LoadInterruptionPoint()
{
	std::ifstream ifs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	if (!ifs)
		return 0;

	ifs.seekg(sizeof(bool) + sizeof(int) + (sizeof(ItemID) + sizeof(int)) * 3 + (sizeof(ItemID) + sizeof(int)) * 24, std::ios_base::beg);
	return ReadData<Point>(ifs);
}

void SaveData::SaveInterruptionPoint(const Point& interruptionPoint)
{
	std::ofstream ofs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	ofs.seekp(sizeof(bool) + sizeof(int) + (sizeof(ItemID) + sizeof(int)) * 3 + (sizeof(ItemID) + sizeof(int)) * 24, std::ios_base::beg);
	WriteData<Point>(ofs, interruptionPoint);
}

Vec2 SaveData::LoadPlayerPosition()
{
	std::ifstream ifs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	ifs.seekg(sizeof(bool) + sizeof(int) + (sizeof(ItemID) + sizeof(int)) * 3 + (sizeof(ItemID) + sizeof(int)) * 24 + sizeof(Point), std::ios_base::beg);
	return ReadData<Vec2>(ifs);
}

void SaveData::SavePlayerPosition(const Vec2& playerPosition)
{
	std::ofstream ofs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	ofs.seekp(sizeof(bool) + sizeof(int) + (sizeof(ItemID) + sizeof(int)) * 3 + (sizeof(ItemID) + sizeof(int)) * 24 + sizeof(Point), std::ios_base::beg);
	WriteData<Point>(ofs, playerPosition);
}

std::vector<std::shared_ptr<MapNode>> SaveData::LoadMap()
{
	std::ifstream ifs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	ifs.seekg(sizeof(bool) + sizeof(int) + (sizeof(ItemID) + sizeof(int)) * 3 + (sizeof(ItemID) + sizeof(int)) * 24 + sizeof(Point) + sizeof(Vec2), std::ios_base::beg);

	unsigned mapSize = ReadData<unsigned>(ifs);

	std::vector<std::shared_ptr<MapNode>> ret_map;
	for (unsigned i = 0; i < mapSize; ++i)
	{
		std::shared_ptr<MapNode> map = std::make_shared<MapNode>();
		map->SetPosition(ReadData<Point>(ifs));
		if (ReadData<bool>(ifs))
			map->Initielized();
		for (int x = 0; x < FIELD_X_NUM; ++x)
			for (int y = 0; y < FIELD_Y_NUM; ++y)
				map->SetFloorMap(x, y, ReadData<char>(ifs));
		map->SetLengthFromBase(ReadData<int>(ifs));

		map->SetRouteLevel(ReadData<int>(ifs));
		map->SetEnemyNum(ReadData<int>(ifs));

		map->SetBossID(ReadData<CharacterID>(ifs));

		ret_map.emplace_back(map);

		for (int dir = 0; dir < static_cast<int>(RouteDirection::RouteNum); ++dir)
		{
			RouteDirection routeDir = static_cast<RouteDirection>(dir);
			RouteState routeState = ReadData<RouteState>(ifs);

			if (routeState == RouteState::None)
			{
				map->SetRoute(routeDir, std::make_shared<Route>());
			}
			else
			{
				//移動先のマップが既に追加されているならば、ルートを設定する
				for (const auto& destination : ret_map)
				{
					if (destination->GetPosition() == map->GetPosition() + myutil::GetPointFromRouteDirection(routeDir))
					{
						mapLayer->CreateRoute(map, routeDir, destination, routeState, false);
						break;
					}
				}
			}
		}
	}

	return ret_map;
}

void SaveData::SaveMap()
{
	std::ofstream ofs(savePath, std::ios::in | std::ios::binary | std::ios::ate);
	ofs.seekp(sizeof(bool) + sizeof(int) + (sizeof(ItemID) + sizeof(int)) * 3 + (sizeof(ItemID) + sizeof(int)) * 24 + sizeof(Point) + sizeof(Vec2), std::ios_base::beg);

	const std::vector<std::shared_ptr<MapNode>>& map = mapLayer->GetMap();

	WriteData<unsigned>(ofs, map.size());

	for (unsigned i = 0; i < map.size(); ++i)
	{
		//マップをセーブ
		WriteData<Point>(ofs, map[i]->GetPosition());
		WriteData<bool>(ofs, map[i]->IsInitFloorMap());
		for (int x = 0; x < FIELD_X_NUM; ++x)
			for (int y = 0; y < FIELD_Y_NUM; ++y)
				WriteData<char>(ofs, map[i]->GetFloorMap()[x][y]);
		WriteData<int>(ofs, map[i]->GetLengthFromBase());
		WriteData<int>(ofs, map[i]->GetRouteLevel());
		WriteData<int>(ofs, map[i]->GetEnemyNum());
		WriteData<CharacterID>(ofs, map[i]->GetBossID());

		for (int dir = 0; dir < static_cast<int>(RouteDirection::RouteNum); ++dir)
		{
			//ルートをセーブ
			WriteData<RouteState>(ofs, map[i]->GetRoute(static_cast<RouteDirection>(dir)).GetState());
		}
	}
}
