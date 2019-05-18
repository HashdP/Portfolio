#pragma once
#include "GameObject/GameObject.hpp"

enum class CharacterID;
enum class SoundID;
class Character;

//�U�����쐬����}�N��
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

	//�_���[�W
	int damage = 0.0f;

	//�t���[�Y
	float freeze = 0.0f;

	//�m�b�N�o�b�N
	float knockback = 0.0f;

	//�q�b�g��
	SoundID hitSoundID;

	bool HitAttack(std::function<bool(const Character&)> func);

	//�����������̃p�[�e�B�N��
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