#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class ObjectLayer;
class GUILayer;

class TitleLayer : public cocos2d::Layer
{
private:
	TitleLayer() = default;

	cocos2d::ui::Button* playButton;
	cocos2d::ui::Button* exitButton;

public:
	static TitleLayer* create(ObjectLayer* objectLayer, GUILayer* guiLayer);
	virtual bool init(ObjectLayer* objectLayer, GUILayer* guiLayer);

	void ClickPlayButton(ObjectLayer* objectLayer, GUILayer* guiLayer);
};
