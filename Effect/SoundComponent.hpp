#pragma once
#include "cocos2d.h"

class SoundID;

class SoundComponent final : public cocos2d::Node
{
private:
	SoundComponent() = default;

	int sound;
	float playInterval;
	float repeatDuration;

	float interval;
	float duration;

	int playingSoundID;

public:
	~SoundComponent();
	static SoundComponent* create(SoundID sound, float interval, float duration);
	bool init(SoundID sound, float interval, float duration);
	void update(float delta) override;

	//������Ԃɖ߂��i�Đ����̃T�E���h����~����j
	void Reset();
};
