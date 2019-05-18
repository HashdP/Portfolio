#pragma once
#include "cocos2d.h"
#include <vector>

class Floor;
class Area;

class MoveAI
{
private:
	float interval = 10000.0f;

	//�T�[�`�Ԋu
	float searchInterval;

	//�ڕW�̃t���A�ɂǂꂾ���߂Â����玟�̃t���A�ɍs����
	float nextRootRange;

	//�T�[�`��������
	std::vector<Floor*> route;

public:

	/*
	�T���f�[�^��ݒ�
	searchInterval�b���ɖړI�n�܂ł̌o�H��T������
	����Floor��nextRouteRange�܂ŋ߂Â����玟��Floor�Ɍ�����
	*/
	void SetData(float searchInterval, float nextRouteRange);

	/*
	fromPos����targetPos�܂ł̌o�H��T����Floor�Ɋi�[����
	moveableWater��true�Ȃ�A�ړ��s�̏ꏊ���܂߂ĒT������
	*/
	bool SearchRoute(const Area& area, const cocos2d::Vec2& fromPos, const cocos2d::Vec2& targetPos, bool moveableWater = false);

	/*
	�֐�����SearchRoute���s���A���ʂ��玟��Floor�܂ł̌������擾����
	zeroUpdate��true�ɂ����ꍇ�A�T�����ʂ�route����ɂȂ����瑦���Ɏ��̒T�����s���B
	*/
	cocos2d::Vec2 MoveUpdate(float delta, const Area& area, const cocos2d::Vec2& fromPos, const cocos2d::Vec2& targetPos, bool moveableWater = false, bool zeroUpdate = false);

	/*
	�ړI�n�ɓ��B���āAroute���󂩂ǂ���
	*/
	inline bool IsArrive() const { return route.empty(); }
};
