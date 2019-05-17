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

	//�t�@�C������T�^�̃f�[�^��ǂݍ���
	template<class T>
	T ReadData(std::ifstream& ifs)
	{
		T buf;
		ifs.read((char*)&buf, sizeof(T));
		return buf;
	}

	//�t�@�C����T�^�̃f�[�^����������
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

	//����
	std::array<Item, 3> LoadWeapon();
	void SaveWeapon();

	//�A�C�e��
	std::array<Item, 24> LoadItem();
	void SaveItem();

	//���f�ꏊ
	cocos2d::Point LoadInterruptionPoint();
	void SaveInterruptionPoint(const cocos2d::Point& interruptionPoint);

	//���}�b�v���̓����
	cocos2d::Vec2 LoadPlayerPosition();
	void SavePlayerPosition(const cocos2d::Vec2& playerPosition);

	//�}�b�v
	std::vector<std::shared_ptr<MapNode>> LoadMap();
	void SaveMap();
};
