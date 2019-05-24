#pragma once
#include "Item.hpp"

class SoundID;
class Player;
class CharaData;

class WeaponItem : public Item
{
private:
	std::string imgFile;
	std::string dataKey;

protected:
	/*
	gunTex　  ：銃のテクスチャー
	gunRect　 ：gunTexのどの位置のテクスチャーを使うか
	gunIsFront：trueならば、銃がプレイヤーのスプライトより前にある
	gunPosDif ：それぞれのアニメーションで銃が中心からどの位置有るかを設定する。sizeがアニメーションのコマ数になる。
	pTex      ：プレイヤーの攻撃アニメーションテクスチャー
	animSpeed ：アニメーションのスピード
	pTexStartX：プレイヤーのアニメーションをpTexのどの位置から始めるか（ｘ）
	pTexStartY：プレイヤーのアニメーションをpTexのどの位置から始めるか（ｙ）
	*/
	cocos2d::Animation* CreateGunAnimation(cocos2d::Texture2D* gunTex, cocos2d::Rect& gunRect, bool gunIsFront, std::vector<cocos2d::Vec2>& gunPosDif, cocos2d::Texture2D* pTex, float animSpeed, int pTexStartX, int pTexStartY);

	/*
	baseKeyのアニメーションが再生されて、bulletTime秒後に銃を撃つ
	soundIDのSEを射撃時に再生する
	*/
	void SetGunFunc(Player& player, std::string baseKey, const CharaData& weaponData, float bulletTiming, SoundID soundID, ItemLayer* itemLayer, ObjectLayer* objectLayer);

public:
	WeaponItem(ItemID id, int num, int maxNum, std::string imgFile, std::string dataKey);
	virtual ~WeaponItem() = default;

	virtual bool Use(int itemIndex, int weaponIndex, ItemLayer* itemLayer, ObjectLayer* objectLayer) override;

	/*
	このアイテムのアニメーションをbaseKeyに設定する
	*/
	virtual void CreateAttackMotion(Player& player, std::string baseKey, ItemLayer* itemLayer, ObjectLayer* objectLayer) {}

	inline std::string GetImgFile() { return imgFile; }
	inline std::string GetDataKey() { return dataKey; }
};
