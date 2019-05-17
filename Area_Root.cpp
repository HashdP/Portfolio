#include "Area_Root.hpp"
#include "Floor.hpp"
#include "MoveAI.hpp"

USING_NS_CC;

Area_Root* Area_Root::create(MapNode& mapNode)
{
	Area_Root* pRet = new(std::nothrow) Area_Root();
	if (pRet && pRet->init(mapNode))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool Area_Root::init(MapNode& mapNode)
{
	if (!Area::init(mapNode))
		return false;

	return true;
}

void Area_Root::InitializeFloor()
{
	//�܂��������ɓ����Ȃ���
	int corY = cocos2d::random<int>(0, 4);
	for (int x = 0; x < FIELD_X_NUM; ++x)
	{
		if (cocos2d::random<int>(0, 2) == 2)
		{
			int dif = cocos2d::random<int>(0, 2) - 1;

			CreateFloor(FloorID::Floor_1, x, corY);

			int nextY = corY + dif;
			if (nextY < 0)
				nextY = 0;
			else if (FIELD_Y_NUM <= nextY)
				nextY = FIELD_Y_NUM - 1;

			corY = nextY;
		}

		CreateFloor(FloorID::Floor_1, x, corY);
	}

	//���T�C�h�̂T�}�X�͕K�����ɂ���
	for (int y = 0; y < FIELD_Y_NUM; ++y)
	{
		CreateFloor(FloorID::Floor_1, 0, y);
		CreateFloor(FloorID::Floor_1, FIELD_X_NUM - 1, y);
	}

	//�c��̕����𖄂߂�
	for (int y = 0; y < FIELD_Y_NUM; ++y)
	{
		for (int x = 0; x < FIELD_X_NUM; ++x)
		{
			if (GetFloor(x, y) == nullptr)
			{
				if (cocos2d::random<int>(0, 1) < 1)
					CreateFloor(FloorID::Floor_1, x, y);
				else
					CreateFloor(FloorID::Water_1, x, y);
			}
		}
	}

	//�����t���A���폜����
	MoveAI moveAI;
	moveAI.SetData(0, 0);
	for (int x = 0; x < FIELD_X_NUM; ++x)
	{
		for (int y = 0; y < FIELD_Y_NUM; ++y)
		{
			if (GetFloor(x, y)->GetMoveable())
			{
				//�v���C���[�̈ʒu�܂ŒT���ł��Ȃ������ꍇ���̏ꏊ���폜����
				if (!moveAI.SearchRoute(*this, GetFloor(FIELD_X_NUM - 1, corY)->getPosition(), GetFloor(x, y)->getPosition()))
					CreateFloor(FloorID::Water_1, x, y);
			}
		}
	}
}
