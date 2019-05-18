#pragma once
#include <string>
#include <unordered_map>

class CharaData
{
private:
	const std::string charaKey;

	static bool initialize;
	static std::unordered_map<std::string, std::unordered_map<std::string, float>> value;

public:
	//filePathから使用するデータをロードする
	CharaData(std::string charaKey);

	//keyから値を取得する
	inline float GetValue(std::string key) const { return value[charaKey].find(key)->second; }

	//keyから値を取得する
	//書き込みのためのオーバロードはしない
	inline float operator[](std::string key) const { return value[charaKey].find(key)->second; }
};
