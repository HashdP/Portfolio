#pragma once
#include "cocos2d.h"

class SoundID;
class ObjectLayer;

class OtherWindowLayer : public cocos2d::Layer
{
private:
	//�ʃE�B���h�E�ŊJ�����C���[�͓����Ɉ���������J���Ȃ�
	static bool windowVisible;

	//�E�B���h�E���J�����Ƃ��̉�
	int openWindowSound;

	virtual void OnVisible(ObjectLayer& objectLayer) {}

protected:
	OtherWindowLayer() = default;
	virtual ~OtherWindowLayer() = default;

	bool init(SoundID soundID);

public:
	void ChangeVisible(ObjectLayer& objectLayer);
};
