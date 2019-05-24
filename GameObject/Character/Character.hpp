#pragma once
#include "GameObject.hpp"
#include "CocosRef.hpp"
#include "SoundComponent.hpp"
#include "EnumWithString.hpp"

class StateAnimationSprite;
class CharaData;

ENUM_CLASS_WITH_STRING(
	CharacterID,
	Player,
	PunchBound,
	PunchBound2,
	PunchBound3,
	ShotBound,
	ShotBound2,
	ShotBound3,
	MagicBound,
	MagicBound2,
	MagicBound3,
	BadBat,
	MossRat,
	Supervisor,
	Skull,
	None
);

class Character : public GameObject
{
private:
	CharacterID charaID;

	StateAnimationSprite* sprite;

	cocos2d::Sprite* shadow;

	std::string state;

	//ヒットボックスはshadowのサイズに対応する
	cocos2d::Rect hitBox;

	//HP
	int HP;
	int maxHP;

	//静止時間
	float freezeTime = 0.0f;

	//色
	cocos2d::Color3B color;

	//ノックバックする割合（1ならすべてノックバック、0ならノックバックしない）
	float knockBackMul = 1.0f;

	//フリーズする割合
	float freezeMul = 1.0f;

	//ダメージを受けるかどうか
	bool ghost = false;

	//サウンドキー
	std::string soundKey;

	//ステイトサウンド
	std::unordered_map<std::string, myutil::cocos_unique_ref<SoundComponent>> stateSounds;

public:
	virtual ~Character() = default;

	virtual bool init(ObjectLayer* objectLayer, CharacterID charaID, float shadowScale = 0.25f);
	virtual bool init(ObjectLayer* objectLayer, CharacterID charaID, const CharaData& charaData);

	//これ以降はFreezeUpdateとCharacterUpdateをオーバーライドする
	virtual void update(float delta) final;

	virtual void CharacterUpdate(float delta) {}

	inline std::string GetObjectType() const override { return "Character"; }

	inline StateAnimationSprite* GetSprite() { return sprite; }

	//キャラクターの移動（壁に当たる）
	virtual void MoveCharacter(cocos2d::Vec2& difPosition);
	
	//freezeTimeだけフリーズさせる
	void Freeze(float freezeTime);

	//フリーズしているかどうか
	bool IsFreeze() { return 0.0f < freezeTime; }

	//ダメージを与える
	virtual void Damage(int damage, float freeze, const cocos2d::Vec2& knockBack);

	inline CharacterID GetCharaID() const { return charaID; }

	virtual void SetState(std::string state, bool changeAnimation = true);
	inline std::string GetState() const { return state; }

	//ヒットボックスを取得
	inline cocos2d::Rect GetHitBox() const { return cocos2d::Rect(getPosition() + hitBox.origin, hitBox.size); }

	//HP
	inline virtual void InitHP(int HP) { this->maxHP = HP; this->HP = HP; }
	inline virtual void SetHP(int HP) { this->HP = HP; }
	inline int GetHP() const    { return HP; }
	inline int GetMaxHP() const { return maxHP; }

	//色
	inline void SetColor(const cocos2d::Color3B& color) { this->color = color; this->setColor(color); }

	//ノックバック
	void SetKnockBack(float knockBack);
	//フリーズ
	void SetFreezeMul(float freezeMul);
	//ダメージを受けるかどうか
	inline void SetGhost(bool flag) { ghost = flag; }
	inline bool IsGhost() const { return ghost; }

	//吹き出しテキスト
	cocos2d::Node* Speak(std::string text, float time, int fontSize = 45);

	//ステイトサウンドを設定する
	void SetStateSound(std::string key, SoundComponent* soundComponent);
};
