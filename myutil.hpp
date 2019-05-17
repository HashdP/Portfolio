#pragma once
#include "cocos2d.h"

enum Direction;
enum RouteDirection;

namespace myutil
{
	//�����𕶎���ɕϊ�����
	std::string GetDirectionStr(Direction direction);

	//���K�����ꂽ�����x�N�g�����擾
	cocos2d::Vec2 GetDirectionVec(Direction direction);

	//toPosition�ւ̂����悻�̌����x�N�g�����擾����
	cocos2d::Vec2 GetDirectionVec8(const cocos2d::Vec2& fromPosition, const cocos2d::Vec2& toPosition);

	//Vec2����Direction���擾
	Direction GetDirectionFromVec(const cocos2d::Vec2& dirVec);

	//Position���牽�Ԗڂ̃t���A�ɋ��邩���擾
	cocos2d::Vec2 ConvertPosToFloorPos(const cocos2d::Vec2& position);

	//FloorPos����Position�֕ϊ�
	cocos2d::Vec2 ConvertFloorPosToPos(int x, int y);

	RouteDirection GetRouteDirectionFromFloorPos(const cocos2d::Vec2& floorPos);

	//RouteDirection�ɑΉ�����Point���擾����
	cocos2d::Point GetPointFromRouteDirection(RouteDirection routeDireciton);

	//�A�j���[�V�������쐬
	cocos2d::Animation* CreateStateAnimation(std::string file, float delay, int beginIndex, int totalNum, int sizeX, int sizeY);

	//�A�j���[�V������ǉ�
	void AddStateAnimation(cocos2d::Animation* animation, std::string file, int beginIndex, int totalNum, int sizeX, int sizeY);

	//base���猩��target���ǂ����ɂ��邩
	Direction GetTargetDir(const cocos2d::Vec2& base, const cocos2d::Vec2& target);

	//vector�̂�����ԍŏ��ɂ���value��j������
	template<class T>
	void EraseVectorValue(std::vector<T>& vec, T value)
	{
		for (unsigned i = 0; i < vec.size(); ++i)
		{
			if (vec[i] == value)
			{
				vec.erase(vec.begin() + i);
				break;
			}
		}
	}

	//Cocos2d��globalZOrder���q�v�f�܂߂��ׂĕύX����
	void SetGlobalZOrderRecursive(cocos2d::Node *parent, float z);
}