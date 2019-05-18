#pragma once
#include "cocos2d.h"
#include "GameObject/Character/Enemy/Enemy.hpp"

class ObjectLayer;

class CharacterFactory
{
private:
	CharacterFactory() = default;
	~CharacterFactory() = default;

	static CharacterFactory* instance;

public:
	CharacterFactory(const CharacterFactory& obj) = delete;
	CharacterFactory& operator=(const CharacterFactory& obj) = delete;
	CharacterFactory(CharacterFactory&& obj) = delete;
	CharacterFactory&& operator=(CharacterFactory&& obj) = delete;

	static CharacterFactory* getInstance();

	Character* CreateNpc(ObjectLayer* objectLayer, CharacterID characterID, const cocos2d::Vec2& position, bool boss = false);
};
