#pragma once
#include "cocos2d.h"
#include "CocosRef.hpp"

class StateAnimationSprite : public cocos2d::Sprite
{
private:
	//アニメーションデータ
	struct AnimationData
	{
		AnimationData() = default;
		AnimationData(bool reverse, cocos2d::Action* animation):reverse(reverse), animation(myutil::make_cocos_unique_ref<cocos2d::Action>(animation)) {}

		//反転しているかどうか
		bool reverse = false;

		//アニメーション
		myutil::cocos_unique_ref<cocos2d::Action> animation = nullptr;

		//アニメーション関数を呼び出すまでのインターバル
		float interval = -1.0f;

		//アニメーション関数
		std::function<void(cocos2d::Node*)> animFunc = nullptr;

		//アニメーション関数をRepeatForeverで複数回呼び出す場合のループの長さ
		float animFuncRepeatInterval = -1.0f;
	};

	StateAnimationSprite() = default;

	std::string animState;

	std::map<std::string, AnimationData> animations;

public:
	~StateAnimationSprite() = default;

	CREATE_FUNC(StateAnimationSprite);
	virtual bool init() override;

	/*
	アニメーションステイトを入力待ちにする
	*/
	inline void ResetAnimationState() { this->animState = ""; }

	/*
	keyにanimationを追加する。
	reverseがtrueならば、アニメーションは反転する。
	endFuncがnullptrなら無限ループ、そうでないならばアニメーション終了時にendFuncが呼ばれる。
	*/
	void RegisterStateAnimation(std::string key, bool reverse, cocos2d::Animation* animation, const std::function<void(Node*)>& endFunc = nullptr);

	/*
	keyにanimFuncをセットする。
	keyのアニメーションが呼ばれてからinterval秒後にanimFuncが呼ばれる
	animFuncRepeatIntervalが正なら、その感覚で続けてanimFuncが呼ばれる
	*/
	void SetAnimFunc(std::string key, float interval, const std::function<void(cocos2d::Node*)>& animFunc, float animFuncRepeatInterval = -1.0f);

	/*
	アニメーションを変更する
	*/
	void SetStateAnimation(std::string key);

	/*
	アニメーションが存在するかどうか
	*/
	inline bool IsExistAnimation(std::string key) const { return animations.find(key) != animations.cend(); }
};
