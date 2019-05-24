#pragma once
#include "Character.hpp"

class GUILayer;
class ItemLayer;

class Player : public Character
{
private:
	Player() = default;

	GUILayer* guiLayer;

	int audioID = -1;

	//プレイヤーの吹き出し
	cocos2d::Node* hukidashi = nullptr;

public:
	static Player* create(ObjectLayer* objectLayer, ItemLayer* itemLayer, const CharaData& charaData);
	virtual bool init(ObjectLayer* objectLayer, ItemLayer* itemLayer, const CharaData& charaData);

	inline void SetGUILayer(GUILayer* guiLayer) { this->guiLayer = guiLayer; }

	void CharacterUpdate(float delta) override;

	inline std::string GetObjectType() const final { return "Player"; }

	virtual void SetState(std::string state, bool changeAnimation = true) override;
	void SetHP(int HP); 

	void MoveCharacter(cocos2d::Vec2& difPosition) override;

	void RemoveWeapon(int weaponIndex);
};
