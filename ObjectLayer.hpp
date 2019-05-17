#pragma once
#include "cocos2d.h"
#include "myutil.hpp"
#include "CocosRef.hpp"

enum RouteDirection;
enum class CharacterID;
class GameObject;
class Character;
class Player;
class WorkshopLayer;
class ItemLayer;
class MapNode;
class ChangeAreaArrow;
class Wall;
class Area;
class SideFloor;

class ObjectLayer : public cocos2d::Layer
{
private:
	ObjectLayer() = default;

	WorkshopLayer* workshopLayer;

	Wall* wall;
	Area* area;
	SideFloor* sideFloor;

	//プレイヤー
	myutil::cocos_shared_ref<Player> player;

	//キャラクター
	std::vector<myutil::cocos_shared_ref<Character>> enemy;

	//フィールドオブジェクト
	std::vector<myutil::cocos_shared_ref<GameObject>> fieldObjects;

	//削除するオブジェクト
	std::vector<myutil::cocos_shared_ref<GameObject>> removeObjects;

	//移動矢印
	std::vector<ChangeAreaArrow*> arrows;

	//現在のマップノード
	std::shared_ptr<MapNode> currentMap;

	bool isRunningUpdate;
	static bool followPlayer;

public:
	static ObjectLayer* create(ItemLayer* itemLayer, WorkshopLayer* workshopLayer);
	virtual bool init(ItemLayer* itemLayer, WorkshopLayer* workshopLayer);
	virtual void update(float delta) override;

	inline const Area& GetArea() const { return *area; }

	//プレイヤー操作
	inline Player& GetPlayer() { return *player; }
	inline const Player& GetConstPlayer() const { return *player; }

	//引数の条件に合うキャラクターのベクターを取得
	cocos2d::Vec2 DamageCharacter(CharacterID parentID, int damage, float freeze, float knockback, Direction direction, std::function<bool(const Character&)> func);

	//キャラクターを取得
	inline bool IsExistEnemy() const { return enemy.empty(); }

	const MapNode GetCurrentMap() const;

	//フィールドオブジェクトを追加
	void AddFieldObject(GameObject* character);
	//オブジェクトを削除する
	void RemoveObject(const GameObject* gameObject);

	//フィールドを移動する
	void MoveField(RouteDirection toDir);
	//フィールドを作成
	void CreateField(const cocos2d::Vec2& playerPosition, std::shared_ptr<MapNode> currentMap, bool firstCrate);

	//アップデートを止める
	void StopUpdate();
	//アップデートを再開する
	void StartUpdate();

	//ルートを解放する
	void ReleaseRoot(RouteDirection rootDir);
};
