#include "Item_AssaultRifle.hpp"
#include "ItemLayer.hpp"
#include "ObjectLayer.hpp"
#include "Player.hpp"
#include "StateAnimationSprite.hpp"
#include "Attack_Bullet.hpp"
#include "EffectFactory.hpp"
#include "SoundManager.hpp"
#include "CharaData.hpp"

USING_NS_CC;

Item_AssaultRifle::Item_AssaultRifle(ItemID id, int num, int maxNum, std::string imgFile, std::string dataKey) :
	WeaponItem(id, num, maxNum, imgFile, dataKey)
{
}

void Item_AssaultRifle::CreateAttackMotion(Player& player, std::string baseKey, ItemLayer* itemLayer, ObjectLayer* objectLayer)
{
	baseKey += "_";
	auto endFunc = [objectLayer](Ref* ref) { objectLayer->GetPlayer().SetState("Stay", false); };
	Texture2D* pAttackTex = Director::getInstance()->getTextureCache()->addImage("Images/Character/player_attack.png");
	Texture2D* gunTex = Director::getInstance()->getTextureCache()->addImage(GetImgFile());
	CharaData weaponData(GetDataKey());

	//前方に射撃
	player.GetSprite()->RegisterStateAnimation(
		baseKey + "Front",
		false,
		CreateGunAnimation(gunTex, Rect(0, 0, 32, 32), false, std::vector<Vec2>({ Vec2(-4, 8), Vec2(-4, 9), Vec2(-4, 7), Vec2(-4, 8), Vec2(-4, 9) }), pAttackTex, weaponData["Spd"], 0, 288),
		endFunc);

	//後ろに射撃
	player.GetSprite()->RegisterStateAnimation(
		baseKey + "Back",
		false,
		CreateGunAnimation(gunTex, Rect(32, 0, 32, 32), false, std::vector<Vec2>({ Vec2(4, 5), Vec2(4, 6), Vec2(4, 8), Vec2(4, 6), Vec2(4, 6) }), pAttackTex, weaponData["Spd"], 0, 320),
		endFunc);

	//左に射撃
	player.GetSprite()->RegisterStateAnimation(
		baseKey + "Left",
		false,
		CreateGunAnimation(gunTex, Rect(64, 0, 32, 32), false, std::vector<Vec2>({ Vec2(-4, 7), Vec2(-4, 8), Vec2(-3, 8), Vec2(-4, 8), Vec2(-4, 8) }), pAttackTex, weaponData["Spd"], 0, 352),
		endFunc);

	//右に射撃
	player.GetSprite()->RegisterStateAnimation(
		baseKey + "Right",
		true,
		CreateGunAnimation(gunTex, Rect(64, 0, 32, 32), true, std::vector<Vec2>({ Vec2(-4, 7), Vec2(-4, 8), Vec2(-3, 8), Vec2(-4, 8), Vec2(-4, 8) }), pAttackTex, weaponData["Spd"], 0, 384),
		endFunc);

	//弾丸とマズルフラッシュ
	SetGunFunc(player, baseKey, weaponData, weaponData["Spd"] * 2, SoundID::AssaultGun_Shot, itemLayer, objectLayer);
}
