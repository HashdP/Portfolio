#pragma once
#include "cocos2d.h"

class GameManager
{
private:
	GameManager() = default;
	~GameManager() = default;

	static GameManager* instance;

	//フェードアウト用のスプライト
	cocos2d::Sprite* overlay;

public:

	static GameManager* getInstance();

	//シーン毎に使用するoverlayをセットする
	inline void SetOverlay(cocos2d::Sprite* overlay) { this->overlay = overlay; }

	//ゲームプレイシーンに移行
	void ReplaceGamePlayScene();

	//エンディングシーンに移行（エンディングナンバーを渡す）
	void ReplaceEndingScene(int endingNumber);
};
