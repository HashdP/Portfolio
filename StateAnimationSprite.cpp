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
	//�����̃A�j���[�V�������폜����
	if (endFunc == nullptr)
	{
		//endState����Ȃ�Ήi�����[�v����
		animations[key] = AnimationData(reverse, RepeatForever::create(Animate::create(animation)));
	}
	else
	{
		//�L���̃A�j���[�V�����̓A�j���I����animationStr��None�ɕύX����
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
	//���݂�state�ƈقȂ�ꍇ�̓A�j���[�V������ύX����
	if (animState != key && animations.find(key) != animations.cend())
	{
		this->stopAllActionsByTag(1);

		//reverse��true�Ȃ��sprite��X���Ώ̂ɂ���
		this->runAction(CallFuncN::create([this, key](Ref* ref) { this->setFlippedX(animations[key].reverse); }));

		//���C���A�j���[�V�������Đ�
		animations[key].animation->setTag(1);
		this->runAction(animations[key].animation.get());

		//AnimFunc���Z�b�g����Ă���ꍇ
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
				//����AnimFunc�����b�Ń��s�[�g����ꍇ
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