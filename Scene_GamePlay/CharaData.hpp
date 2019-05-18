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
	//filePath����g�p����f�[�^�����[�h����
	CharaData(std::string charaKey);

	//key����l���擾����
	inline float GetValue(std::string key) const { return value[charaKey].find(key)->second; }

	//key����l���擾����
	//�������݂̂��߂̃I�[�o���[�h�͂��Ȃ�
	inline float operator[](std::string key) const { return value[charaKey].find(key)->second; }
};
