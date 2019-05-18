#include "Factory/CharacterFactory.hpp"
#include "Scene_GamePlay/Layer/ObjectLayer.hpp"
#include "Scene_GamePlay/Light.hpp"
#include "GameObject/Character/Enemy/Bound/PunchBound.hpp"
#include "GameObject/Character/Enemy/Bound/ShotBound.hpp"
#include "GameObject/Character/Enemy/Bound/MagicBound.hpp"
#include "GameObject/Character/Enemy/BadBat/BadBat.hpp"
#include "GameObject/Character/Enemy/MossRat/MossRat.hpp"
#include "GameObject/Character/Enemy/NPC/Supervisor.hpp"
#include "GameObject/Character/Enemy/NPC/Skull.hpp"
#include "Scene_GamePlay/CharaData.hpp"

USING_NS_CC;

CharacterFactory* CharacterFactory::instance = nullptr;

CharacterFactory* CharacterFactory::getInstance()
{
	if (instance == nullptr)
		instance = new CharacterFactory();
	return instance;
}

Character* CharacterFactory::CreateNpc(ObjectLayer* objectLayer, CharacterID characterID, const Vec2& position, bool boss)
{
	Character* npc = nullptr;

	switch (characterID)
	{
	case CharacterID::PunchBound:  npc = PunchBound::create(objectLayer, characterID, 1, CharaData("PunchBound")); break;
	case CharacterID::PunchBound2: npc = PunchBound::create(objectLayer, characterID, 2, CharaData("PunchBound2")); break;
	case CharacterID::PunchBound3: npc = PunchBound::create(objectLayer, characterID, 3, CharaData("PunchBound3")); break;

	case CharacterID::ShotBound:  npc = ShotBound::create(objectLayer, characterID, 1, CharaData("ShotBound")); break;
	case CharacterID::ShotBound2: npc = ShotBound::create(objectLayer, characterID, 2, CharaData("ShotBound2")); break;
	case CharacterID::ShotBound3: npc = ShotBound::create(objectLayer, characterID, 3, CharaData("ShotBound3")); break;

	case CharacterID::MagicBound:  npc = MagicBound::create(objectLayer, characterID, 1, CharaData("MagicBound")); break;
	case CharacterID::MagicBound2: npc = MagicBound::create(objectLayer, characterID, 2, CharaData("MagicBound2")); break;
	case CharacterID::MagicBound3: npc = MagicBound::create(objectLayer, characterID, 3, CharaData("MagicBound3")); break;

	case CharacterID::BadBat:      npc = BadBat::create(objectLayer, CharaData("BadBat"));    break;

	case CharacterID::MossRat:     npc = MossRat::create(objectLayer, CharaData("MossRat"));  break;

	case CharacterID::Supervisor:  npc = Supervisor::create(objectLayer); break;

	case CharacterID::Skull:       npc = Skull::create(objectLayer); break;
	}

	if (boss)
	{
		//É{ÉXÇÃèÍçá
		Enemy* enemy = static_cast<Enemy*>(npc);
		if (enemy)
		{
			enemy->SetKnockBack(0.0f);
			enemy->SetFreezeMul(0.0f);
			enemy->SetBossFlag(true);
			enemy->InitHP(enemy->GetHP() * 2.5f);
		}
	}

	npc->setPosition(position);
	objectLayer->AddFieldObject(npc);

	return npc;
}
