#include "Item/Item_Aid.hpp"
#include "GameObject/Character/Player.hpp"
#include "Scene_GamePlay/Layer/ObjectLayer.hpp"
#include "Manager/SoundManager.hpp"

Item_Aid::Item_Aid(ItemID id, int num, int maxNum, int value) :
	Item(id, num, maxNum),
	value(value)
{
}

bool Item_Aid::Use(int itemIndex, int weaponIndex, ItemLayer* itemLayer, ObjectLayer* objectLayer)
{
	Player& player = objectLayer->GetPlayer();

	SoundManager::getInstance()->Play2DSound(SoundID::UseItem_Spray);
	player.SetHP(player.GetMaxHP());

	return true;
}
