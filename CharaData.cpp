#include "CharaData.hpp"
#include <fstream>
#include <sstream>

bool CharaData::initialize = false;
std::unordered_map<std::string, std::unordered_map<std::string, float>> CharaData:: value;

CharaData::CharaData(std::string charaKey) :
	charaKey(charaKey)
{
	if (!initialize)
	{
		//最初の一回でロードする
		std::ifstream ifs("Data/CharaData.txt");

		std::string str;
		while (std::getline(ifs, str))
		{
			std::istringstream iss(str);

			std::string mainKey;
			std::getline(iss, mainKey, ':');

			if (mainKey == "CharaID")
			{
				//CharaIDを見つける
				std::string mainKeyValue;
				std::getline(iss, mainKeyValue, ':');

				//CharaIDのキーに値を格納する
				std::string valueStr;
				while (std::getline(ifs, valueStr))
				{
					if(valueStr == "/")
						break;

					std::istringstream valueIss(valueStr);

					std::string key;
					std::getline(valueIss, key, ':');

					std::string fValue;
					std::getline(valueIss, fValue);

					value[mainKeyValue][key] = std::atof(fValue.c_str());
				}
			}
		}

		initialize = true;
	}
}
