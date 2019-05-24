#include "SoundManager.hpp"
#include <sqlite3.h>

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
	//データベースからサウンドデータを読み込む

	sqlite3* sql3;
	if (sqlite3_open("Data/SoundData.db", &sql3) == SQLITE_OK)
	{
		sqlite3_stmt* statement;
		const char* command = "select * from SoundData";

		sqlite3_prepare_v2(sql3, command, -1, &statement, &command);
		while (sqlite3_step(statement) == SQLITE_ROW)
		{
			SoundID soundID = SoundID::ToIndex(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
			std::string filePath = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
			float volume = sqlite3_column_double(statement, 2);

			PreLoad(soundID, filePath, volume);
		}

		sqlite3_finalize(statement);
		sqlite3_close(sql3);
	}
}

void SoundManager::PreLoad(SoundID soundID, std::string filePath, float volume)
{
	sounds[soundID] = std::pair <std::string, float>(filePath, volume);
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
