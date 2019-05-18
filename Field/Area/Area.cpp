#include "Field/Area/Area.hpp"
#include "Scene_GamePlay/MapNode.hpp"
#include "myutil.hpp"
#include "Scene_GamePlay/Floor.hpp"
#include "GameObject/GameObject.hpp"

USING_NS_CC;

bool Area::init(MapNode& mapNode)
{
	if (!Node::init())
		return false;

	if (mapNode.IsInitFloorMap())
	{
		//既に初期化されている場合
		const std::array<std::array<char, FIELD_Y_NUM>, FIELD_X_NUM>& floorMap = mapNode.GetFloorMap();

		for (int x = 0; x < FIELD_X_NUM; ++x)
			for (int y = 0; y < FIELD_Y_NUM; ++y)
			{
				switch (floorMap[x][y])
				{
				case 'f': CreateFloor(FloorID::Floor_1, x, y); break;
				case 'w': CreateFloor(FloorID::Water_1, x, y); break;
				}
			}
	}
	else
	{
		InitializeFloor();

		mapNode.Initielized();
		mapNode.SetFloorMap(floors);
	}

	CreateFloorEvent();

	return true;
}

Floor* Area::CreateFloor(FloorID floorID, int x, int y)
{
	Floor* addFloor = nullptr;

	switch (floorID)
	{
	case FloorID::Floor_1: addFloor = Floor::create(true, "Images/Field/floor_1.png", 0.5f, 2.0f / 3.0f); break;
	case FloorID::Water_1: addFloor = Floor::create(false, "Images/Field/water_1.png", 0.5f, 0.9f); break;
	}

	if (addFloor != nullptr)
	{
		addFloor->SetFloorID(floorID);

		if (floors[x][y] != nullptr)
			this->removeChild(floors[x][y], true);

		//Backの床をセット
		if (0 < y)
			addFloor->SetNextFloor(floors[x][y - 1], Direction::Back);
		//Frontの床をセット
		if (y < FIELD_Y_NUM - 1)
			addFloor->SetNextFloor(floors[x][y + 1], Direction::Front);
		//Leftの床をセット
		if (0 < x)
			addFloor->SetNextFloor(floors[x - 1][y], Direction::Left);
		//Rightの床をセット
		if (x < FIELD_X_NUM - 1)
			addFloor->SetNextFloor(floors[x + 1][y], Direction::Right);

		addFloor->setPosition(-304 + FIELD_SQUARE_SIZE * x, -64 + FIELD_SQUARE_SIZE * y);
		addFloor->setLocalZOrder(4 - y);

		floors[x][y] = addFloor;
		this->addChild(addFloor);
	}

	return addFloor;
}

Floor* Area::GetFloor(const Vec2& position) const
{
	Vec2 floorPos = myutil::ConvertPosToFloorPos(position);

	if (0 <= floorPos.x && 0 <= floorPos.y)
		return floors[floorPos.x][floorPos.y];
	else
		return nullptr;
}

std::vector<Floor*> Area::GetFloorVector() const
{
	std::vector<Floor*> floorVector;

	for (int x = 0; x < FIELD_X_NUM; ++x)
		for (int y = 0; y < FIELD_Y_NUM; ++y)
			floorVector.emplace_back(floors[x][y]);

	return floorVector;
}

const Vec2 Area::GetRandomMoveablePlace(bool center) const
{
	Floor* randomFloor = nullptr;
	while (!randomFloor || !randomFloor->GetMoveable())
	{
		int randX = random<int>(3, FIELD_X_NUM - 4);
		int randY = random<int>(0, FIELD_Y_NUM - 1);

		randomFloor = floors[randX][randY];
	}

	Vec2 randomPlace = randomFloor->getPosition();

	if (!center)
		randomPlace += Vec2(random<float>(-10.0f, 10.0f), random<float>(-10.0f, 10.0f));

	return randomPlace;
}

bool Area::IsMoveablePlace(const Vec2& position) const
{
	Vec2 floorPos = myutil::ConvertPosToFloorPos(position);

	if (position.x < -320 || FIELD_X_NUM <= floorPos.x)
		return false;
	if (position.y < -80 || FIELD_Y_NUM <= floorPos.y)
		return false;

	return floors[floorPos.x][floorPos.y]->GetMoveable();
}
