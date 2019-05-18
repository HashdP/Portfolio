#pragma once
#include "GameObject/Character/Character.hpp"

class HPGauge;
class MoveAI;

class Enemy : public Character
{
private:
	std::unique_ptr<MoveAI> moveAI;

	HPGauge* hpGauge;

	//ƒ{ƒX‚Ìê‡
	bool bossFlag;

protected:
	Enemy() = default;

	inline const MoveAI& GetMoveAI() const { return *moveAI; }
	cocos2d::Vec2 AIUpdate(float delta, const cocos2d::Vec2& target, bool moveableWater = false, bool zeroUpdate = false);
	void SearchRoute(const cocos2d::Vec2& target, bool moveableWater = false);

public:
	virtual ~Enemy() = default;
	virtual bool init(ObjectLayer* objectLayer, CharacterID charaID, const CharaData& charaData, float searchInterval, float nextRootRange);
	inline std::string GetObjectType() const final { return "Enemy"; }

	virtual void Damage(int damage, float freeze, const cocos2d::Vec2& knockBack) override;

	inline void SetBossFlag(bool bossFlag) { this->bossFlag = bossFlag; }
	inline bool GetBossFlag() const { return bossFlag; }
};
