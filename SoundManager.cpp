#include "SoundManager.hpp"

USING_NS_CC;
using namespace experimental;

SoundManager* SoundManager::instance = nullptr;

SoundManager* SoundManager::getInstance()
{
	if (!instance)
		instance = new SoundManager();

	return instance;
}

void SoundManager::LoadSound()
{
	PreLoad(SoundID::Background,        "cave1.mp3", 0.3f);
	PreLoad(SoundID::Player_Walk,       "walk-leather-shoes1.mp3", 0.2f);
	PreLoad(SoundID::Player_Run,        "dash-leather-shoes1.mp3", 0.2f);
	PreLoad(SoundID::Player_Punch,      "punch-swing1.mp3", 0.5f);
	PreLoad(SoundID::Bound_Move,        "slime1.mp3", 0.5f);
	PreLoad(SoundID::Bound_Attack,      "gum1.mp3", 0.3f);
	PreLoad(SoundID::HandGun_Shot,      "M1 Garand Single-SoundBible.com-1941178963.mp3", 0.4f);
	PreLoad(SoundID::AssaultGun_Shot,   "M4A1_Single-Kibblesbob-8540445.mp3", 0.1f);
	PreLoad(SoundID::AirShot,           "airshot.mp3", 0.15f);
	PreLoad(SoundID::HitSound_PlayerPunch, "punch-middle2.mp3", 0.5f);
	PreLoad(SoundID::HitSound_BoundPunch, "se_maoudamashii_battle12.mp3", 0.3f);
	PreLoad(SoundID::HitSound_Bullet,   "hitbullet.mp3", 1.0f);
	PreLoad(SoundID::HitSound_DirtShot, "se_maoudamashii_battle18.mp3", 0.4f);
	PreLoad(SoundID::GetItem,           "se_maoudamashii_system48.mp3", 0.3f);
	PreLoad(SoundID::UseItem,           "equipment.mp3", 0.4f);
	PreLoad(SoundID::CannotUseItem,     "cancel2.mp3", 0.4f);
	PreLoad(SoundID::UseItem_Spray,     "spray.mp3", 0.4f);
	PreLoad(SoundID::ChangeItemCursor,  "cursor8.mp3", 0.3f);
	PreLoad(SoundID::ChangeWeaponCursor, "changeweapon.mp3", 0.3f);
	PreLoad(SoundID::ClimbLadder,       "climbladder.mp3", 0.3f);
	PreLoad(SoundID::Activate_Workshop, "jail-door-close1.mp3", 0.6f);
	PreLoad(SoundID::Activate_Map,      "openmap.mp3", 1.0f);
	PreLoad(SoundID::Hukidashi_Appear,  "cursor4.mp3", 1.0f);

	PreLoad(SoundID::Title_Play, "decision16.mp3", 0.3f);

	PreLoad(SoundID::Ending_Result_Appear, "drum-japanese1.mp3", 0.4f);
	PreLoad(SoundID::Ending_Text_Appear, "keyboard1.mp3", 0.4f);
}

void SoundManager::PreLoad(SoundID soundID, std::string filePath, float volume)
{
	sounds[soundID] = std::pair < std::string, float>(filePath, volume);
	cocos2d::experimental::AudioEngine::preload("Sound/" + filePath);
}

int SoundManager::Play2DSound(SoundID soundID, bool loop, float volume)
{
	if (volume < 0.0f)
	{
		//volumeがマイナスならデフォルトボリュームを使用する
		return cocos2d::experimental::AudioEngine::play2d("Sound/" + sounds[soundID].first, loop, sounds[soundID].second);
	}
	else
	{
		return cocos2d::experimental::AudioEngine::play2d("Sound/" + sounds[soundID].first, loop, volume);
	}
}
