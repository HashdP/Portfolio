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

	//SoundIDとそれに対応するFilePathとVolumeを設定する
	void PreLoad(SoundID soundID, std::string filePath, float volume);

public:
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(const SoundManager&&) = delete;
	SoundManager&& operator=(const SoundManager&&) = delete;

	static SoundManager* getInstance();

	//ゲームで使用するすべてのサウンドをPreLoadする。
	void LoadSound();

	//サウンドを再生する。Volume未設定なら、PreLoadで設定したVolumeを使用する
	//戻り値はユニークなID。Stopなどに使用する
	int Play2DSound(SoundID soundID, bool loop = false, float volume = -1.0f);

	//全てのサウンドを停止する
	inline void StopAllSound() { cocos2d::experimental::AudioEngine::stopAll(); }

	//ID（SoundIDではなくPlay2DSoundの戻り値）を使用してサウンドを停止する
	inline void StopSound(int id) { cocos2d::experimental::AudioEngine::stop(id); }
};
