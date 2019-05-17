#include "Item_Weapon.hpp"
#include "PlayerData.hpp"

Item_Weapon::Item_Weapon(ItemID id, int num, int maxNum) :
	Item(id, num, maxNum)
{
}

bool Item_Weapon::Use(PlayerData* playerData)
{
	Item* curWeapon = playerData->GetWeapon(playerData->GetWeaponIndex());
	if (curWeapon)
		playerData->AddItem(curWeapon->GetID(), 1);

	playerData->AddWeapon(GetID(), playerData->GetWeaponIndex());

	return true;
}
