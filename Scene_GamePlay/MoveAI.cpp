#include "Scene_GamePlay/MoveAI.hpp"
#include "Field/Area/Area.hpp"
#include "Scene_GamePlay/Floor.hpp"
#include "GameObject/GameObject.hpp"
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

	//���߂�Floor�̃R�X�g���O�ɂ���
	startFloor->SetCost(0);

	while (true)
	{
		Floor* toFloor = nullptr;

		for (Floor* floor : floors)
		{
			//�܂��T�����Ă��Ȃ��^�T���ς݂̏ꍇ
			if (floor->GetDone() || floor->GetCost() < 0)
				continue;

			//processFloor�����ݒ�̏ꍇ
			if (!toFloor)
			{
				toFloor = floor;
				continue;
			}

			if (floor->GetCost() < toFloor->GetCost())
				toFloor = floor;
		}

		//���ׂĒT�����I�������T�����I������
		if (!toFloor)
			break;

		toFloor->SetDone(true);

		for (Floor* floor : toFloor->GetNextFloor())
		{
			//�ׂɈړ��\��Floor������ꍇ
			if (floor != nullptr && (floor->GetMoveable() || moveableWater))
			{
				//�K�v�ȃR�X�g�i�G�b�W�̏d�݂͂��ׂĂP�j
				int needCost = toFloor->GetCost() + 1;

				if (floor->GetCost() < 0 || needCost < floor->GetCost())
				{
					floor->SetCost(needCost);
					floor->SetFromFloor(toFloor);
				}
			}
		}
	}

	//route�ɐݒ肷��
	route.clear();

	Floor* reserveFloor = area.GetFloor(targetPos);
	while (true)
	{
		if (reserveFloor == startFloor)
			break;
		if (reserveFloor == nullptr)
		{
			//�T�����s
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
