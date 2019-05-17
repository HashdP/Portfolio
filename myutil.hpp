#pragma once
#include "cocos2d.h"

enum Direction;
enum RouteDirection;

namespace myutil
{
	//方向を文字列に変換する
	std::string GetDirectionStr(Direction direction);

	//正規化された向きベクトルを取得
	cocos2d::Vec2 GetDirectionVec(Direction direction);

	//toPositionへのおおよその向きベクトルを取得する
	cocos2d::Vec2 GetDirectionVec8(const cocos2d::Vec2& fromPosition, const cocos2d::Vec2& toPosition);

	//Vec2からDirectionを取得
	Direction GetDirectionFromVec(const cocos2d::Vec2& dirVec);

	//Positionから何番目のフロアに居るかを取得
	cocos2d::Vec2 ConvertPosToFloorPos(const cocos2d::Vec2& position);

	//FloorPosからPositionへ変換
	cocos2d::Vec2 ConvertFloorPosToPos(int x, int y);

	RouteDirection GetRouteDirectionFromFloorPos(const cocos2d::Vec2& floorPos);

	//RouteDirectionに対応するPointを取得する
	cocos2d::Point GetPointFromRouteDirection(RouteDirection routeDireciton);

	//アニメーションを作成
	cocos2d::Animation* CreateStateAnimation(std::string file, float delay, int beginIndex, int totalNum, int sizeX, int sizeY);

	//アニメーションを追加
	void AddStateAnimation(cocos2d::Animation* animation, std::string file, int beginIndex, int totalNum, int sizeX, int sizeY);

	//baseから見てtargetがどっちにあるか
	Direction GetTargetDir(const cocos2d::Vec2& base, const cocos2d::Vec2& target);

	//vectorのうち一番最初にあるvalueを破棄する
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

	//Cocos2dのglobalZOrderを子要素含めすべて変更する
	void SetGlobalZOrderRecursive(cocos2d::Node *parent, float z);
}