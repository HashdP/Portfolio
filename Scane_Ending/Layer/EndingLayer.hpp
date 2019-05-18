#pragma once
#include "cocos2d.h"
#include <fstream>

class Character;

class EndingLayer : public cocos2d::Layer
{
private:
	bool animEnd;
	bool processActive;

	std::ifstream ifs;

	Character* mainCharaSprite;
	std::string hukidashiText;
	cocos2d::Label* mainLabel;
	std::vector<cocos2d::Sprite*> letters;

	//文字アニメーション
	void LetterAnimation();

	//アニメーションを終了
	void EndAnim();

public:
	static EndingLayer* create(int endingNumber);

	virtual bool init(int endingNumber);
	virtual void update(float delta) override;
};
