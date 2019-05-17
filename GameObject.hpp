#pragma once
#include "cocos2d.h"

class ObjectLayer;

//ObjectLayer*を引数にとるcocos2dのCREATE_FUNC
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

//ゲームオブジェクト
class GameObject : public cocos2d::Node
{
private:
	ObjectLayer* objectLayer;

	//trueならOnRemoveが呼ばれる。
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
	ObjectTypeを取得
	*/
	virtual std::string GetObjectType() const { return ""; }

	/*
	オブジェクトフィールド上のY座標
	*/
	inline virtual float GetFieldPositionY() const { return getPositionY(); }

	/*
	ObjectLayerでRemoveObjectを使用してオブジェクトを削除した場合、呼ばれる。
	*/
	virtual void OnRemove() {}
};