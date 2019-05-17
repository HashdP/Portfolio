#pragma once
#include "cocos2d.h"

class ObjectLayer;

//ObjectLayer*�������ɂƂ�cocos2d��CREATE_FUNC
#define CREATE_FUNC_WITH_OBJECTLAYER(__TYPE__) \
static __TYPE__* create(ObjectLayer* objectLayer) \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init(objectLayer)) \
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

//�Q�[���I�u�W�F�N�g
class GameObject : public cocos2d::Node
{
private:
	ObjectLayer* objectLayer;

	//true�Ȃ�OnRemove���Ă΂��B
	bool onRemove;

protected:
	GameObject() = default;

	inline ObjectLayer* GetObjectLayer() { return objectLayer; }

public:
	virtual ~GameObject() = default;

	virtual bool init(ObjectLayer* objectLayer);

	inline void SetOnRemove(bool onRemove) { this->onRemove = onRemove; }
	inline bool GetOnRemove() const { return onRemove; }

	/*
	ObjectType���擾
	*/
	virtual std::string GetObjectType() const { return ""; }

	/*
	�I�u�W�F�N�g�t�B�[���h���Y���W
	*/
	inline virtual float GetFieldPositionY() const { return getPositionY(); }

	/*
	ObjectLayer��RemoveObject���g�p���ăI�u�W�F�N�g���폜�����ꍇ�A�Ă΂��B
	*/
	virtual void OnRemove() {}
};