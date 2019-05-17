#pragma once
#include "CocosRef.hpp"
#include <string>
#include <array>
#include <memory>

class ItemLayer;
class MapLayer;
class Item;
class MapNode;

class SaveData
{
private:
	SaveData();
	~SaveData() = default;

	static SaveData* instance;

	std::string savePath;

	myutil::cocos_unique_ref<ItemLayer> itemLayer;
	myutil::cocos_unique_ref<MapLayer> mapLayer;

	//ファイルからT型のデータを読み込む
	template<class T>
	T ReadData(std::ifstream& ifs)
	{
		T buf;
		ifs.read((char*)&buf, sizeof(T));
		return buf;
	}

	//ファイルにT型のデータを書き込む
	template<class T>
	void WriteData(std::ofstream& ofs, T data)
	{
		ofs.write((const char*)&data, sizeof(T));
	}

public:
	SaveData(const SaveData& obj) = delete;
	SaveData& operator=(const SaveData& obj) = delete;
	SaveData(SaveData&& obj) = delete;
	SaveData& operator=(SaveData&& obj) = delete;

	static SaveData* getInstance();

	void init(ItemLayer* itemLayer, MapLayer* mapLayer);
	void finalize();

	bool IsExistSaveFile();

	//ContinueFlag
	bool LoadContinueFlag();
	void SaveContinueFlag(bool flag);

	//EndingNumber
	int LoadEndingNumber();
	void SaveEndingNumber(int endingNumber);

	//武器
	std::array<Item, 3> LoadWeapon();
	void SaveWeapon();

	//アイテム
	std::array<Item, 24> LoadItem();
	void SaveItem();

	//中断場所
	cocos2d::Point LoadInterruptionPoint();
	void SaveInterruptionPoint(const cocos2d::Point& interruptionPoint);

	//同マップ内の入り口
	cocos2d::Vec2 LoadPlayerPosition();
	void SavePlayerPosition(const cocos2d::Vec2& playerPosition);

	//マップ
	std::vector<std::shared_ptr<MapNode>> LoadMap();
	void SaveMap();
};
