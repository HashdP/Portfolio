#pragma once
#include "GameObject/GameObject.hpp"

enum class CharacterID;
enum class SoundID;
class Character;

//攻撃を作成するマクロ
#define ATTACK_CREATE_FUNC(__TYPE__) \
static __TYPE__* create(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float dmg, float frz, float kb, SoundID hitSEID) \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init(parentID, objectLayer, originPosition, direction, dmg, frz, kb, hitSEID)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}

class Attack : public GameObject
{
private:
	CharacterID parentID;

	//ダメージ
	int damage = 0.0f;

	//フリーズ
	float freeze = 0.0f;

	//ノックバック
	float knockback = 0.0f;

	//ヒット音
	SoundID hitSoundID;

	bool HitAttack(std::function<bool(const Character&)> func);

	//当たった時のパーティクル
	virtual void HitSparkle(const cocos2d::Vec2& position) {}

protected:
	Attack() = default;
	bool HitAttack(const cocos2d::Rect& attackRect);
	bool HitAttack(cocos2d::Vec2& attackPosition, float radius);

public:
	virtual ~Attack() = default;
	virtual bool init(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float dmg, float frz, float kb, SoundID hitSEID);

	inline std::string GetObjectType() const final { return "Attack"; }
};