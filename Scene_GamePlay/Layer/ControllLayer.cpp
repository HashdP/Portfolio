#include "ControllLayer.hpp"
#include "MapLayer.hpp"
#include "TutorialLayer.hpp"
#include "ObjectLayer.hpp"
#include "ItemLayer.hpp"
#include "GUILayer.hpp"
#include "Floor.hpp"
#include "Area.hpp"
#include "Player.hpp"
#include "InputManager.hpp"
#include "SoundManager.hpp"

USING_NS_CC;

bool ControllLayer::isRunningUpdate = true;

ControllLayer* ControllLayer::create(ItemLayer* itemLayer, ObjectLayer* objectLayer, GUILayer* guiLayer, MapLayer* mapLayer, TutorialLayer* tutorialLayer)
{
	ControllLayer* pRet = new(std::nothrow) ControllLayer();
	if (pRet && pRet->init(itemLayer, objectLayer, guiLayer, mapLayer, tutorialLayer))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool ControllLayer::init(ItemLayer* itemLayer, ObjectLayer* objectLayer, GUILayer* guiLayer, MapLayer* mapLayer, TutorialLayer* tutorialLayer)
{
	if (!Layer::init())
		return false;

	scheduleUpdate();

	this->itemLayer = itemLayer;
	this->objectLayer = objectLayer;
	this->guiLayer = guiLayer;
	this->mapLayer = mapLayer;
	this->tutorialLayer = tutorialLayer;

	SetItem(itemIndex, false);
	SetWeapon(weaponIndex, false);

	//GUI�Ɋ֐���ݒ肷��
	guiLayer->SetGUIBackgroundFunc([this](Ref* sender, ui::Widget::TouchEventType type) { if (type == ui::Widget::TouchEventType::BEGAN && isRunningUpdate) SetItem(-1); });
	for (unsigned i = 0; i < itemLayer->GetMaxItemNum(); ++i)
		guiLayer->SetItemBoxFunc(i, [this, itemLayer, i](Ref* sender, ui::Widget::TouchEventType type) { if (type == ui::Widget::TouchEventType::BEGAN && isRunningUpdate) SetItem(i); });
	for (unsigned i = 0; i < itemLayer->GetMaxWeaponNum(); ++i)
		guiLayer->SetWeaponBoxFunc(i, [this, itemLayer, i](Ref* sender, ui::Widget::TouchEventType type) { if (type == ui::Widget::TouchEventType::BEGAN && isRunningUpdate) SetWeapon(i); });

	return true;
}

void ControllLayer::update(float delta)
{
	if (isRunningUpdate)
	{
		Player& player = objectLayer->GetPlayer();
		if (!player.IsFreeze() && player.GetState() != "Attack0" && player.GetState() != "Attack1" && player.GetState() != "Attack2")
		{
			//�ړ�����
			Vec2 dirVec = Vec2::ZERO;
			if (InputManager::getInstance()->GetInput(InputCode::KEY_W)) dirVec.y += 1;
			if (InputManager::getInstance()->GetInput(InputCode::KEY_A)) dirVec.x -= 1;
			if (InputManager::getInstance()->GetInput(InputCode::KEY_S)) dirVec.y -= 1;
			if (InputManager::getInstance()->GetInput(InputCode::KEY_D)) dirVec.x += 1;
			dirVec.normalize();

			//���������߂�i�΂߂ɓ����Ă���ꍇ�͂������Ɍ����Ă��邱�ƂƂ���j
			Direction dir = myutil::GetDirectionFromVec(dirVec);
			if (dir != Direction::Num)
				player.SetDirection(dir);

			//�U�������ꍇ
			if (InputManager::getInstance()->GetInput(InputCode::MOUSE_LEFT_FIELD))
			{
				player.SetState("Attack" + std::to_string(weaponIndex));
				return;
			}

			//state��ݒ�
			if (dirVec != Vec2::ZERO)
			{
				if (InputManager::getInstance()->GetInput(InputCode::KEY_SHIFT))
				{
					player.SetState("Run");
					player.MoveCharacter(dirVec * 1.5f);
				}
				else
				{
					player.SetState("Walk");
					player.MoveCharacter(dirVec * 0.5f);
				}
			}
			else
			{
				player.SetState("Stay");
			}
		}

		//�����ύX����
		if (0 < InputManager::getInstance()->GetMouseScroll())
			SetWeapon((weaponIndex + 1) % 3);
		else if (InputManager::getInstance()->GetMouseScroll() < 0)
			SetWeapon((weaponIndex + 2) % 3);

		//�A�C�e�����̂Ă�
		if (InputManager::getInstance()->GetInputDown(KEY_C) && itemIndex != -1)
		{
			Vec2 dirVec = myutil::GetDirectionVec(objectLayer->GetConstPlayer().GetDirection());
			Item currentItem = itemLayer->GetItem(itemIndex);
			ItemFactory::getInstance()->CreateDropItem(objectLayer, objectLayer->GetConstPlayer().getPosition() + dirVec * 20, currentItem.GetID(), currentItem.GetNum());
			itemLayer->RemoveItem(itemIndex);
		}

		//�񕜂���i�񕜖���������Ă���ꍇ�j
		if (InputManager::getInstance()->GetInputDown(KEY_Q))
		{
			int index = itemLayer->IsExistItem(ItemID::Aid, 1);
			if (index != -1)
			{
				itemLayer->UseItem(index, weaponIndex, objectLayer);
			}
		}
	}

	//�������
	if (InputManager::getInstance()->GetInputDown(KEY_T) && (isRunningUpdate || tutorialLayer->isVisible()))
	{
		tutorialLayer->ChangeVisible(*objectLayer);
	}

	//�}�b�v
	if (InputManager::getInstance()->GetInputDown(KEY_M) && (isRunningUpdate || mapLayer->isVisible()))
	{
		mapLayer->ChangeVisible(*objectLayer);
	}

	//�H�[���j���[�^�A�C�e���̎g�p
	if (InputManager::getInstance()->GetInputDown(KEY_E))
	{
		//�t���A�C�x���g������ꍇ
		if (objectLayer->GetArea().IsMoveablePlace(objectLayer->GetConstPlayer().getPosition()))
		{
			Floor* floor = objectLayer->GetArea().GetFloor(objectLayer->GetConstPlayer().getPosition());
			if (floor->IsFloorEventActive() && floor->FloorEvent())
				return;
		}

		if (itemIndex != -1 && isRunningUpdate)
		{
			if (itemLayer->GetItem(itemIndex).GetID() != ItemID::None)
			{
				itemLayer->UseItem(itemIndex, weaponIndex, objectLayer);
			}
			else
			{
				//���ݑI�����Ă���Item��None�̏ꍇ�A��������O��

				Item currentWeapon = itemLayer->GetWeapon(weaponIndex);
				if (currentWeapon.GetID() != ItemID::None)
				{
					SoundManager::getInstance()->Play2DSound(SoundID::UseItem);
					itemLayer->EquipWeapon(itemIndex, weaponIndex);
					objectLayer->GetPlayer().RemoveWeapon(weaponIndex);
				}
				else
				{
					SoundManager::getInstance()->Play2DSound(SoundID::CannotUseItem);
				}
			}
			return;
		}
	}
}

void ControllLayer::SetItem(int index, bool soundFlag)
{
	itemIndex = index;
	itemLayer->SetItem(index, soundFlag);
}

void ControllLayer::SetWeapon(int index, bool soundFlag)
{
	if (index < 0) index = 0;
	else if (3 <= index) index = 2;

	weaponIndex = index;
	itemLayer->SetWeapon(index, soundFlag);
}
