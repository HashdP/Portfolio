#pragma once
#include "cocos2d.h"

class GameManager
{
private:
	GameManager() = default;
	~GameManager() = default;

	static GameManager* instance;

	//�t�F�[�h�A�E�g�p�̃X�v���C�g
	cocos2d::Sprite* overlay;

public:

	static GameManager* getInstance();

	//�V�[�����Ɏg�p����overlay���Z�b�g����
	inline void SetOverlay(cocos2d::Sprite* overlay) { this->overlay = overlay; }

	//�Q�[���v���C�V�[���Ɉڍs
	void ReplaceGamePlayScene();

	//�G���f�B���O�V�[���Ɉڍs�i�G���f�B���O�i���o�[��n���j
	void ReplaceEndingScene(int endingNumber);
};
