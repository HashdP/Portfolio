#pragma once
#include "cocos2d.h"
#include "CocosRef.hpp"

class StateAnimationSprite : public cocos2d::Sprite
{
private:
	//�A�j���[�V�����f�[�^
	struct AnimationData
	{
		AnimationData() = default;
		AnimationData(bool reverse, cocos2d::Action* animation) : reverse(reverse), animation(myutil::make_cocos_unique_ref<cocos2d::Action>(animation)) {}

		//���]���Ă��邩�ǂ���
		bool reverse = false;

		//�A�j���[�V����
		myutil::cocos_unique_ref<cocos2d::Action> animation = nullptr;

		//�A�j���[�V�����֐����Ăяo���܂ł̃C���^�[�o��
		float interval = -1.0f;

		//�A�j���[�V�����֐�
		std::function<void(cocos2d::Node*)> animFunc = nullptr;

		//�A�j���[�V�����֐���RepeatForever�ŕ�����Ăяo���ꍇ�̃��[�v�̒���
		float animFuncRepeatInterval = -1.0f;
	};

	StateAnimationSprite() = default;

	std::string animState;

	std::map<std::string, AnimationData> animations;

public:
	~StateAnimationSprite() = default;

	CREATE_FUNC(StateAnimationSprite);
	virtual bool init() override;

	//�A�j���[�V�����X�e�C�g
	inline void SetAnimationState(std::string animState) { this->animState = animState; }

	//�A�j���[�V������o�^
	void RegisterStateAnimation(std::string key, bool reverse, cocos2d::Animation* animation, const std::function<void(Node*)>& endFunc = nullptr);

	//AnimFunc��ݒ肷��
	void SetAnimFunc(std::string key, float interval, const std::function<void(cocos2d::Node*)>& animFunc, float animFuncRepeatInterval = -1.0f);

	//�A�j���[�V������ύX����
	void SetStateAnimation(std::string key);

	//�A�j���[�V���������݂��邩�ǂ���
	inline bool IsExistAnimation(std::string key) const { return animations.find(key) != animations.cend(); }
};
