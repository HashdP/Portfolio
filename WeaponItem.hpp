#pragma once
#include "Item.hpp"

enum class SoundID;
class Player;
class CharaData;

class WeaponItem : public Item
{
private:
	std::string imgFile;
	std::string dataKey;

protected:
	cocos2d::Animation* CreateGunAnimation(cocos2d::Texture2D* gunTex, cocos2d::Rect& gunRect, bool gunIsFront, std::vector<cocos2d::Vec2>& gunPosDif, cocos2d::Texture2D* pTex, float animSpeed, int pTexStartX, int pTexStartY);
	void SetGunFunc(Player& player, std::string baseKey, const CharaData& weaponData, float bulletTiming, SoundID soundID, ItemLayer* itemLayer, ObjectLayer* objectLayer);

public:
	WeaponItem(ItemID id, int num, int maxNum, std::string imgFile, std::string dataKey);
	virtual ~WeaponItem() = default;

	virtual bool Use(int itemIndex, int weaponIndex, ItemLayer* itemLayer, ObjectLayer* objectLayer) override;

	virtual void CreateAttackMotion(Player& player, std::string baseKey, ItemLayer* itemLayer, ObjectLayer* objectLayer) {}

	inline std::string GetImgFile() { return imgFile; }
	inline std::string GetDataKey() { return dataKey; }
};
