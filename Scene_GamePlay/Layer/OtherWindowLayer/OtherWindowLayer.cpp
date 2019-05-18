#include "Scene_GamePlay/Layer/OtherWindowLayer/OtherWindowLayer.hpp"
#include "Scene_GamePlay/Layer/ObjectLayer.hpp"
#include "Manager/SoundManager.hpp"

bool OtherWindowLayer::windowVisible = false;

bool OtherWindowLayer::init(SoundID soundID)
{
	if (!Layer::init())
		return false;

	this->openWindowSound = soundID;

	this->setVisible(false);

	return true;
}

void OtherWindowLayer::ChangeVisible(ObjectLayer& objectLayer)
{
	bool isVisible = !(this->isVisible());

	if (!isVisible || (isVisible && !windowVisible))
	{
		this->setVisible(isVisible);

		windowVisible = isVisible;

		if (isVisible)
		{
			OnVisible(objectLayer);

			objectLayer.setVisible(false);
			objectLayer.StopUpdate();
			SoundManager::getInstance()->Play2DSound(openWindowSound);
		}
		else
		{
			objectLayer.setVisible(true);
			objectLayer.StartUpdate();
		}
	}
}
