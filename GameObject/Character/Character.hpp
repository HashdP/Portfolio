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

	//�q�b�g�{�b�N�X��shadow�̃T�C�Y�ɑΉ�����
	cocos2d::Rect hitBox;

	//HP
	int HP;
	int maxHP;

	//�Î~����
	float freezeTime = 0.0f;

	//�F
	cocos2d::Color3B color;

	//�m�b�N�o�b�N���銄���i1�Ȃ炷�ׂăm�b�N�o�b�N�A0�Ȃ�m�b�N�o�b�N���Ȃ��j
	float knockBackMul = 1.0f;

	//�t���[�Y���銄��
	float freezeMul = 1.0f;

	//�_���[�W���󂯂邩�ǂ���
	bool ghost = false;

	//�T�E���h�L�[
	std::string soundKey;

	//�X�e�C�g�T�E���h
	std::unordered_map<std::string, myutil::cocos_unique_ref<SoundComponent>> stateSounds;

public:
	virtual ~Character() = default;

	virtual bool init(ObjectLayer* objectLayer, CharacterID charaID, float shadowScale = 0.25f);
	virtual bool init(ObjectLayer* objectLayer, CharacterID charaID, const CharaData& charaData);

	//����ȍ~��FreezeUpdate��CharacterUpdate���I�[�o�[���C�h����
	virtual void update(float delta) final;

	virtual void CharacterUpdate(float delta) {}

	inline std::string GetObjectType() const override { return "Character"; }

	inline StateAnimationSprite* GetSprite() { return sprite; }

	//�L�����N�^�[�̈ړ��i�ǂɓ�����j
	virtual void MoveCharacter(cocos2d::Vec2& difPosition);
	
	//freezeTime�����t���[�Y������
	void Freeze(float freezeTime);

	//�t���[�Y���Ă��邩�ǂ���
	bool IsFreeze() { return 0.0f < freezeTime; }

	//�_���[�W��^����
	virtual void Damage(int damage, float freeze, const cocos2d::Vec2& knockBack);

	inline CharacterID GetCharaID() const { return charaID; }

	virtual void SetState(std::string state, bool changeAnimation = true);
	inline std::string GetState() const { return state; }

	//�q�b�g�{�b�N�X���擾
	inline cocos2d::Rect GetHitBox() const { return cocos2d::Rect(getPosition() + hitBox.origin, hitBox.size); }

	//HP
	inline virtual void InitHP(int HP) { this->maxHP = HP; this->HP = HP; }
	inline virtual void SetHP(int HP) { this->HP = HP; }
	inline int GetHP() const    { return HP; }
	inline int GetMaxHP() const { return maxHP; }

	//�F
	inline void SetColor(const cocos2d::Color3B& color) { this->color = color; this->setColor(color); }

	//�m�b�N�o�b�N
	void SetKnockBack(float knockBack);
	//�t���[�Y
	void SetFreezeMul(float freezeMul);
	//�_���[�W���󂯂邩�ǂ���
	inline void SetGhost(bool flag) { ghost = flag; }
	inline bool IsGhost() const { return ghost; }

	//�����o���e�L�X�g
	cocos2d::Node* Speak(std::string text, float time, int fontSize = 45);

	//�X�e�C�g�T�E���h��ݒ肷��
	void SetStateSound(std::string key, SoundComponent* soundComponent);
};
