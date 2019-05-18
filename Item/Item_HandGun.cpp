#include "Item/Item_HandGun.hpp"
#include "GameObject/Character/Player.hpp"
#include "GameObject/Attack/Attack_Bullet.hpp"
#include "Scene_GamePlay/Layer/ItemLayer.hpp"
#include "Scene_GamePlay/Layer/ObjectLayer.hpp"
#include "StateAnimationSprite.hpp"
#include "Factory/EffectFactory.hpp"
#include "Manager/SoundManager.hpp"
#include "Scene_GamePlay/CharaData.hpp"

USING_NS_CC;

Item_HandGun::Item_HandGun(ItemID id, int num, int maxNum, std::string imgFile, std::string dataKey) :
	WeaponItem(id, num, maxNum, imgFile, dataKey)
{
}

void Item_HandGun::CreateAttackMotion(Player& player, std::string baseKey, ItemLayer* itemLayer, ObjectLayer* objectLayer)
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
		CreateGunAnimation(gunTex, Rect(0, 0, 16, 16), true, std::vector<Vec2>({ Vec2(0, 8), Vec2(0, 8), Vec2(0, 6), Vec2(0, 7), Vec2(0, 8) }), pAttackTex, weaponData["Spd"], 0, 192),
		endFunc);

	//後方に射撃
	player.GetSprite()->RegisterStateAnimation(
		baseKey + "Back", 
		false, 
		myutil::CreateStateAnimation("Images/Character/player_attack.png", weaponData["Spd"], 35, 5, 32, 32),
		endFunc);

	//左に射撃
	player.GetSprite()->RegisterStateAnimation(
		baseKey + "Left",
		false,
		CreateGunAnimation(gunTex, Rect(16, 0, 16, 16), true, std::vector<Vec2>({ Vec2(-7, 0), Vec2(-7, 1), Vec2(-5, 1), Vec2(-6, 1), Vec2(-7, 0) }), pAttackTex, weaponData["Spd"], 0, 256),
		endFunc);

	//右に射撃
	player.GetSprite()->RegisterStateAnimation(
		baseKey + "Right",
		true,
		CreateGunAnimation(gunTex, Rect(16, 0, 16, 16), true, std::vector<Vec2>({ Vec2(-7, 0), Vec2(-7, 1), Vec2(-5, 1), Vec2(-6, 1), Vec2(-7, 0) }), pAttackTex, weaponData["Spd"], 0, 256),
		endFunc);

	//弾丸やマズルフラッシュ
	SetGunFunc(player, baseKey, weaponData, weaponData["Spd"] * 2, SoundID::HandGun_Shot, itemLayer, objectLayer);
}
