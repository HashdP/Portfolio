#pragma once
#include "cocos2d.h"

enum class SoundID;
class ObjectLayer;

class OtherWindowLayer : public cocos2d::Layer
{
private:
	//別ウィンドウで開くレイヤーは同時に一つだけしか開かない
	static bool windowVisible;

	//ウィンドウを開いたときの音
	SoundID openWindowSound;

	virtual void OnVisible(ObjectLayer& objectLayer) {}

protected:
	OtherWindowLayer() = default;
	virtual ~OtherWindowLayer() = default;

	bool init(SoundID soundID);

public:
	void ChangeVisible(ObjectLayer& objectLayer);
};
