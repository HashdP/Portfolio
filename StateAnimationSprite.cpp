#include "StateAnimationSprite.hpp"
#include "SoundManager.hpp"

USING_NS_CC;

bool StateAnimationSprite::init()
{
	if (!Sprite::init())
		return false;

	return true;
}

void StateAnimationSprite::RegisterStateAnimation(std::string key, bool reverse, Animation* animation, const std::function<void(Node*)>& endFunc)
{
	//既存のアニメーションを削除する
	if (endFunc == nullptr)
	{
		//endStateが空ならば永遠ループする
		animations[key] = AnimationData(reverse, RepeatForever::create(Animate::create(animation)));
	}
	else
	{
		//有限のアニメーションはアニメ終わりにanimationStrをNoneに変更する
		animations[key] = AnimationData(
			reverse, 
			Sequence::create(
				Animate::create(animation),
				CallFuncN::create(endFunc),
				CallFuncN::create([this](Node*) {ResetAnimationState(); }),
				NULL
			)
		);
	}
}

void StateAnimationSprite::SetAnimFunc(std::string key, float interval, const std::function<void(cocos2d::Node*)>& animFunc, float animFuncRepeatInterval)
{
	animations[key].interval = interval;
	animations[key].animFunc = animFunc;
	animations[key].animFuncRepeatInterval = animFuncRepeatInterval;
}

void StateAnimationSprite::SetStateAnimation(std::string key)
{
	//現在のstateと異なる場合はアニメーションを変更する
	if (animState != key && animations.find(key) != animations.cend())
	{
		this->stopAllActionsByTag(1);

		//reverseがtrueならばspriteをX軸対称にする
		this->runAction(CallFuncN::create([this, key](Ref* ref) { this->setFlippedX(animations[key].reverse); }));

		//メインアニメーションを再生
		animations[key].animation->setTag(1);
		this->runAction(animations[key].animation.get());

		//AnimFuncがセットされている場合
		if (0 <= animations[key].interval && animations[key].animFunc != nullptr)
		{
			Sequence* funcSeq =
				Sequence::create(
					DelayTime::create(animations[key].interval),
					CallFuncN::create(animations[key].animFunc),
					NULL
				);

			Action* funcAction = funcSeq;

			if (0.0f < animations[key].animFuncRepeatInterval)
			{
				//もしAnimFuncを一定秒でリピートする場合
				funcAction =
					RepeatForever::create(
						Spawn::createWithTwoActions(
							funcSeq,
							DelayTime::create(animations[key].animFuncRepeatInterval)
						)
					);
			}

			funcAction->setTag(1); 
			this->runAction(funcAction);
		}

		animState = key;
	}
}