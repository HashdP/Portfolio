#pragma once
#include "AudioEngine.h"
#include "EnumWithString.hpp"
#include <unordered_map>

ENUM_WITH_STRING
(
	SoundID,

	Background,
	Player_Walk,
	Player_Run,
	Player_Punch,
	Bound_Move,
	Bound_Attack,
	Bound_Shot,
	Bound_Magic,
	BadBat_Shot,
	MossRat_Move,
	MossRat_Attack,
	MossRat_Magic,
	HandGun_Shot,
	AssaultGun_Shot,
	AirShot,
	HitSound_PlayerPunch,
	HitSound_BoundPunch,
	HitSound_MossRatPunch,
	HitSound_Bullet,
	HitSound_DirtShot,
	GetItem,
	UseItem,
	CannotUseItem,
	UseItem_Spray,
	ChangeItemCursor,
	ChangeWeaponCursor,
	ClimbLadder,
	Activate_Workshop,
	Activate_Map,
	Hukidashi_Appear,

	Title_Play,

	Ending_Result_Appear,
	Ending_Text_Appear,

	Num
);

//enum class SoundID
//{
//	Background,
//	Player_Walk,
//	Player_Run,
//	Player_Punch,
//	Bound_Move,
//	Bound_Attack,
//	Bound_Shot,
//	Bound_Magic,
//	BadBat_Shot,
//	MossRat_Move,
//	MossRat_Attack,
//	MossRat_Magic,
//	HandGun_Shot,
//	AssaultGun_Shot,
//	AirShot,
//	HitSound_PlayerPunch,
//	HitSound_BoundPunch,
//	HitSound_MossRatPunch,
//	HitSound_Bullet,
//	HitSound_DirtShot,
//	GetItem,
//	UseItem,
//	CannotUseItem,
//	UseItem_Spray,
//	ChangeItemCursor,
//	ChangeWeaponCursor,
//	ClimbLadder,
//	Activate_Workshop,
//	Activate_Map,
//	Hukidashi_Appear,
//
//	Title_Play,
//
//	Ending_Result_Appear,
//	Ending_Text_Appear,
//
//	Num
//};

class SoundManager
{
private:
	SoundManager() = default;
	~SoundManager() = default;

	static SoundManager* instance;

	std::unordered_map<int, std::pair<std::string, float>> sounds;

	//SoundID�Ƃ���ɑΉ�����FilePath��Volume��ݒ肷��
	void PreLoad(SoundID soundID, std::string filePath, float volume);

public:
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(const SoundManager&&) = delete;
	SoundManager&& operator=(const SoundManager&&) = delete;

	static SoundManager* getInstance();

	//�Q�[���Ŏg�p���邷�ׂẴT�E���h��PreLoad����B
	void LoadSound();

	//�T�E���h���Đ�����BVolume���ݒ�Ȃ�APreLoad�Őݒ肵��Volume���g�p����
	//�߂�l�̓��j�[�N��ID�BStop�ȂǂɎg�p����
	int Play2DSound(SoundID soundID, bool loop = false, float volume = -1.0f);

	//�S�ẴT�E���h���~����
	inline void StopAllSound() { cocos2d::experimental::AudioEngine::stopAll(); }

	//ID�iSoundID�ł͂Ȃ�Play2DSound�̖߂�l�j���g�p���ăT�E���h���~����
	inline void StopSound(int id) { cocos2d::experimental::AudioEngine::stop(id); }
};
