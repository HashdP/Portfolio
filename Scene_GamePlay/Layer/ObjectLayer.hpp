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

	//�v���C���[
	myutil::cocos_shared_ref<Player> player;

	//�L�����N�^�[
	std::vector<myutil::cocos_shared_ref<Character>> enemy;

	//�t�B�[���h�I�u�W�F�N�g
	std::vector<myutil::cocos_shared_ref<GameObject>> fieldObjects;

	//�폜����I�u�W�F�N�g
	std::vector<myutil::cocos_shared_ref<GameObject>> removeObjects;

	//�ړ����
	std::vector<ChangeAreaArrow*> arrows;

	//���݂̃}�b�v�m�[�h
	std::shared_ptr<MapNode> currentMap;

	bool isRunningUpdate;
	static bool followPlayer;

public:
	static ObjectLayer* create(ItemLayer* itemLayer, WorkshopLayer* workshopLayer);
	virtual bool init(ItemLayer* itemLayer, WorkshopLayer* workshopLayer);
	virtual void update(float delta) override;

	inline const Area& GetArea() const { return *area; }

	//�v���C���[����
	inline Player& GetPlayer() { return *player; }
	inline const Player& GetConstPlayer() const { return *player; }

	//�����̏����ɍ����L�����N�^�[�̃x�N�^�[���擾
	cocos2d::Vec2 DamageCharacter(CharacterID parentID, int damage, float freeze, float knockback, Direction direction, std::function<bool(const Character&)> func);

	//�L�����N�^�[���擾
	inline bool IsExistEnemy() const { return enemy.empty(); }

	const MapNode GetCurrentMap() const;

	//�t�B�[���h�I�u�W�F�N�g��ǉ�
	void AddFieldObject(GameObject* character);
	//�I�u�W�F�N�g���폜����
	void RemoveObject(const GameObject* gameObject);

	//�t�B�[���h���ړ�����
	void MoveField(RouteDirection toDir);
	//�t�B�[���h���쐬
	void CreateField(const cocos2d::Vec2& playerPosition, std::shared_ptr<MapNode> currentMap, bool firstCrate);

	//�A�b�v�f�[�g���~�߂�
	void StopUpdate();
	//�A�b�v�f�[�g���ĊJ����
	void StartUpdate();

	//���[�g���������
	void ReleaseRoot(RouteDirection rootDir);
};
