#include "WeaponItem.hpp"
#include "ItemLayer.hpp"
#include "ObjectLayer.hpp"
#include "CharaData.hpp"
#include "Player.hpp"
#include "StateAnimationSprite.hpp"
#include "SoundManager.hpp"
#include "Attack_Bullet.hpp"
#include "EffectFactory.hpp"

USING_NS_CC;

WeaponItem::WeaponItem(ItemID id, int num, int maxNum, std::string handgunImgFile, std::string dataKey) :
	Item(id, num, maxNum),
	imgFile(handgunImgFile),
	dataKey(dataKey)
{
}

bool WeaponItem::Use(int itemIndex, int weaponIndex, ItemLayer* itemLayer, ObjectLayer* objectLayer)
{
	itemLayer->EquipWeapon(itemIndex, weaponIndex);

	//アニメーションを作成する
	CreateAttackMotion(objectLayer->GetPlayer(), "Attack" + std::to_string(weaponIndex), itemLayer, objectLayer);

	return true;
}

Animation* WeaponItem::CreateGunAnimation(cocos2d::Texture2D* gunTexture, cocos2d::Rect& gunRect, bool gunIsFront, std::vector<cocos2d::Vec2>& gunPosDif, cocos2d::Texture2D* pTex, float animSpeed, int pTexStartX, int pTexStartY)
{
	Animation* animation = Animation::create();

	for (unsigned i = 0; i < gunPosDif.size(); ++i)
	{
		RenderTexture* tex = RenderTexture::create(32, 32);

		tex->begin();

		//銃（後ろにある場合）
		if (!gunIsFront)
		{
			Sprite* gunSprite = Sprite::createWithTexture(gunTexture, gunRect);
			gunSprite->setPosition(Vec2(16, 16) + gunPosDif[i]);
			gunSprite->setScaleY(-1);
			gunSprite->visit();
		}

		//プレイヤー
		Sprite* p = Sprite::createWithTexture(pTex, Rect(pTexStartX + 32 * i, pTexStartY, 32, 32));
		p->setPosition(16, 16);
		p->setScaleY(-1);
		p->visit();

		//銃（前にある場合）
		if (gunIsFront)
		{
			Sprite* gunSprite = Sprite::createWithTexture(gunTexture, gunRect);
			gunSprite->setPosition(Vec2(16, 16) + gunPosDif[i]);
			gunSprite->setScaleY(-1);
			gunSprite->visit();
		}

		tex->end();

		tex->getSprite()->getTexture()->setAntiAliasTexParameters();
		animation->addSpriteFrameWithTexture(tex->getSprite()->getTexture(), Rect(0, 0, 32, 32));
	}

	animation->setDelayPerUnit(animSpeed);

	return animation;
}

void WeaponItem::SetGunFunc(Player& player, std::string baseKey, const CharaData& weaponData, float bulletTiming, SoundID soundID, ItemLayer* itemLayer, ObjectLayer* objectLayer)
{
	for (int i = 0; i < (int)Direction::Num; ++i)
	{
		Direction dir = (Direction)i;

		player.GetSprite()->SetAnimFunc(
			baseKey + myutil::GetDirectionStr(dir),
			bulletTiming,
			[dir, weaponData, soundID, itemLayer, objectLayer](Node* node)
		{
			if (itemLayer->RemoveItem(ItemID::Bullet, 1))
			{
				//マズルフラッシュ
				Node* muzzleFlash = EffectFactory::getInstance()->CreateEffect(objectLayer, EffectID::MuzzleFlash);

				switch (dir)
				{
				case Direction::Left: muzzleFlash->setPosition(objectLayer->GetConstPlayer().getPosition() + Vec2(-10, 12)); break;
				case Direction::Right: muzzleFlash->setPosition(objectLayer->GetConstPlayer().getPosition() + Vec2(10, 12)); break;
				case Direction::Back: muzzleFlash->setPosition(objectLayer->GetConstPlayer().getPosition() + Vec2(0, 18)); break;
				case Direction::Front: muzzleFlash->setPosition(objectLayer->GetConstPlayer().getPosition()); break;
				}

				//弾丸
				Attack_Bullet* bullet = Attack_Bullet::create(CharacterID::Player, objectLayer, objectLayer->GetConstPlayer().getPosition(), dir, weaponData["Atk"], weaponData["Frz"], weaponData["Kb"], SoundID::HitSound_Bullet);
				objectLayer->AddFieldObject(bullet);

				SoundManager::getInstance()->Play2DSound(soundID);
			}
			else
			{
				SoundManager::getInstance()->Play2DSound(SoundID::AirShot);
			}
		});
	}
}
