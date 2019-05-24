#include "SoundComponent.hpp"
#include "SoundManager.hpp"

USING_NS_CC;

SoundComponent::~SoundComponent()
{
	Reset();
}

SoundComponent* SoundComponent::create(SoundID soundID, float playInterval, float repeatDuration)
{
	SoundComponent* pRet = new(std::nothrow)SoundComponent();
	if (pRet && pRet->init(soundID, playInterval, repeatDuration))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool SoundComponent::init(SoundID soundID, float playInterval, float repeatDuration)
{
	if (!Node::init())
		return false;

	this->sound = soundID;
	this->playInterval = playInterval;
	this->repeatDuration = repeatDuration;

	interval = 0.0f;
	duration = 0.0f;

	playingSoundID = -1;

	return true;
}

void SoundComponent::update(float delta)
{
	if (sound != SoundID::Num && playInterval <= interval)
	{
		if (duration == 0.0f || repeatDuration <= duration)
		{
			playingSoundID = SoundManager::getInstance()->Play2DSound(sound);
			duration = 0.0f;
		}

		duration += delta;
	}

	interval += delta;
}

void SoundComponent::Reset()
{
	if (playingSoundID != -1)
	{
		SoundManager::getInstance()->StopSound(playingSoundID);
	}

	duration = 0.0f;
	interval = 0.0f;
}