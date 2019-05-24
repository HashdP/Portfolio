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
	gunTex�@  �F�e�̃e�N�X�`���[
	gunRect�@ �FgunTex�̂ǂ̈ʒu�̃e�N�X�`���[���g����
	gunIsFront�Ftrue�Ȃ�΁A�e���v���C���[�̃X�v���C�g���O�ɂ���
	gunPosDif �F���ꂼ��̃A�j���[�V�����ŏe�����S����ǂ̈ʒu�L�邩��ݒ肷��Bsize���A�j���[�V�����̃R�}���ɂȂ�B
	pTex      �F�v���C���[�̍U���A�j���[�V�����e�N�X�`���[
	animSpeed �F�A�j���[�V�����̃X�s�[�h
	pTexStartX�F�v���C���[�̃A�j���[�V������pTex�̂ǂ̈ʒu����n�߂邩�i���j
	pTexStartY�F�v���C���[�̃A�j���[�V������pTex�̂ǂ̈ʒu����n�߂邩�i���j
	*/
	cocos2d::Animation* CreateGunAnimation(cocos2d::Texture2D* gunTex, cocos2d::Rect& gunRect, bool gunIsFront, std::vector<cocos2d::Vec2>& gunPosDif, cocos2d::Texture2D* pTex, float animSpeed, int pTexStartX, int pTexStartY);

	/*
	baseKey�̃A�j���[�V�������Đ�����āAbulletTime�b��ɏe������
	soundID��SE���ˌ����ɍĐ�����
	*/
	void SetGunFunc(Player& player, std::string baseKey, const CharaData& weaponData, float bulletTiming, SoundID soundID, ItemLayer* itemLayer, ObjectLayer* objectLayer);

public:
	WeaponItem(ItemID id, int num, int maxNum, std::string imgFile, std::string dataKey);
	virtual ~WeaponItem() = default;

	virtual bool Use(int itemIndex, int weaponIndex, ItemLayer* itemLayer, ObjectLayer* objectLayer) override;

	/*
	���̃A�C�e���̃A�j���[�V������baseKey�ɐݒ肷��
	*/
	virtual void CreateAttackMotion(Player& player, std::string baseKey, ItemLayer* itemLayer, ObjectLayer* objectLayer) {}

	inline std::string GetImgFile() { return imgFile; }
	inline std::string GetDataKey() { return dataKey; }
};
