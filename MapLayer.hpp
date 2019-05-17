#pragma once
#include "OtherWindowLayer.hpp"

enum RouteDirection;
enum class RouteState;
enum class CharacterID;
class MapNode;
class ObjectLayer;

/*
cocos2d::Pointをunordered_mapで使うためのハッシュ関数オブジェクト
*/
struct PointHash
{
	size_t operator()(const cocos2d::Point& point) const
	{
		auto xHash = std::hash<float>()(point.x);
		auto yHash = std::hash<float>()(point.y);

		//boostのhash_combineの実装をコピペ
		size_t seed = 0;
		seed ^= xHash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= yHash + 0x9e3779b9 + (seed << 6) + (seed >> 2);

		return seed;
	}
};

class MapLayer : public OtherWindowLayer
{
private:
	MapLayer() = default;

	std::vector<std::shared_ptr<MapNode>> map;

	std::unordered_map<cocos2d::Point, cocos2d::Sprite*, PointHash> mapSquare;
	std::unordered_map<cocos2d::Point, cocos2d::Sprite*, PointHash> routeLine;

	//マップノードをつなげるルートを作成する
	bool CreateRandomRoute(std::shared_ptr<MapNode> from, std::shared_ptr<MapNode> to, RouteState state);

	void OnVisible(ObjectLayer& objectLayer) override;

public:
	CREATE_FUNC(MapLayer);
	bool init() override;

	std::shared_ptr<MapNode> GetBaseMap() { return map[0]; }

	const std::vector<std::shared_ptr<MapNode>>& GetMap() { return map; }
	const std::shared_ptr<MapNode>& GetMap(const cocos2d::Point& position);

	void CreateRoute(std::shared_ptr<MapNode> from, RouteDirection routeDir, std::shared_ptr<MapNode> to, RouteState state, bool setToProperty = true);
};