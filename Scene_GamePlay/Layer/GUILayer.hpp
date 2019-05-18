#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <array>

class PlayerData;
class ObjectLayer;
class MapDisplay;
class MapNode;
class Player;

class GUILayer : public cocos2d::Layer
{
private:
	GUILayer() = default;

	cocos2d::Sprite* HPGauge;

	cocos2d::ui::Button* guiBackground;
	std::array<cocos2d::ui::Button*, 24> itemBoxes;
	std::array<cocos2d::ui::Button*, 3> weponBoxes;

	cocos2d::ui::Button* goLeftArrow;
	cocos2d::ui::Button* goRightArrow;

	cocos2d::Sprite* itemBoxLight;
	cocos2d::Sprite* weaponBoxLight;

public:
	static GUILayer* create();
	virtual bool init();

	void UpdateHPGauge(int hp, int maxhp);

	//GUI�̃{�^���֐���ݒ�
	void SetGUIBackgroundFunc(const std::function<void(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)>& func);
	void SetItemBoxFunc(int index, const std::function<void(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)>& func);
	void SetWeaponBoxFunc(int index, const std::function<void(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)>& func);

	//�A�C�e���摜��ǉ�����
	void AddItemImage(cocos2d::Sprite* sprite, int index);
	//�A�C�e���̑I�����C�g
	void SetItemBoxLight(int index);

	//�����ǉ�����
	void AddWeaponImage(cocos2d::Sprite* sprite, int index);
	//����̑I�����C�g
	void SetWeaponBoxLight(int index);

	//�Q�[�W���쐬
	cocos2d::Node* CreateGauge(cocos2d::Sprite* &gaugeSprite, const cocos2d::Color3B& gaugeColor);

	//�A�C�e���{�b�N�X�̏ꏊ���擾
	cocos2d::Vec2 GetItemBoxPosition(int index);
	//����{�b�N�X�̏ꏊ���擾
	cocos2d::Vec2 GetWeaponBoxPosition(int index);
};
