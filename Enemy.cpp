#include "Enemy.hpp"
#include "ObjectLayer.hpp"
#include "HPGauge.hpp"
#include "MoveAI.hpp"
#include "CharaData.hpp"

USING_NS_CC;

bool Enemy::init(ObjectLayer* objectLayer, CharacterID charaID, const CharaData& charaData, float searchInterval, float nextRootRange)
{
	if (!Character::init(objectLayer, charaID, charaData))
		return false;

	hpGauge = HPGauge::create();
	hpGauge->setVisible(false);
	hpGauge->setPositionY(charaData["HPGaugeDif"]);
	hpGauge->setGlobalZOrder(1);
	this->addChild(hpGauge);

	moveAI.reset(new MoveAI());
	moveAI->SetData(searchInterval, nextRootRange);

	return true;
}

Vec2 Enemy::AIUpdate(float delta, const Vec2& target, bool moveableWater, bool zeroUpdate)
{
	return moveAI->MoveUpdate(delta, GetObjectLayer()->GetArea(), this->getPosition(), target, moveableWater, zeroUpdate);
}

void Enemy::SearchRoute(const cocos2d::Vec2& target, bool moveableWater)
{
	moveAI->SearchRoute(GetObjectLayer()->GetArea(), this->getPosition(), target, moveableWater);
}

void Enemy::Damage(int damage, float freeze, const cocos2d::Vec2& knockBack)
{
	if (0 < damage)
		hpGauge->Visible(2.0f);
	hpGauge->SetHP(GetHP() - damage, GetMaxHP());

	Character::Damage(damage, freeze, knockBack);

	if (GetHP() <= 0)
	{
		//Ž€‚ñ‚¾‚çíœ‚·‚é
		GetObjectLayer()->RemoveObject(this);
	}
}