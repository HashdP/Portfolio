#pragma execution_character_set("utf-8")

#include "Scene_GamePlay/ChangeAreaArrow.hpp"
#include "Scene_GamePlay/MapNode.hpp"
#include "Scene_GamePlay/Route.hpp"

USING_NS_CC;

ChangeAreaArrow* ChangeAreaArrow::create(RouteDirection routeDirection, RouteState routeState)
{
	ChangeAreaArrow* pRet = new ChangeAreaArrow();

	if (pRet && pRet->init(routeDirection, routeState))
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

bool ChangeAreaArrow::init(RouteDirection routeDirection, RouteState routeState)
{
	if (!Node::init())
		return false;

	this->routeDirection = routeDirection;

	arrow = Node::create();
	this->addChild(arrow);

	//矢印
	Texture2D* arrowTex = Director::getInstance()->getTextureCache()->addImage("Images/move_arrow.png");
	Sprite* arrowSprite = Sprite::createWithTexture(arrowTex);
	arrowSprite->setScale(0.2f);
	arrowSprite->setFlippedX(routeDirection == RouteDirection::LowerLeft || routeDirection == RouteDirection::UpperLeft);
	arrowSprite->runAction(
		RepeatForever::create(
			Sequence::create(
				MoveBy::create(0.2f, Vec2(-5, 0)),
				MoveBy::create(0.2f, Vec2(5, 0)),
				NULL
			)
		)
	);
	arrow->addChild(arrowSprite);

	//テキスト
	Label* text = nullptr;
	switch (routeDirection)
	{
	case RouteDirection::UpperLeft:  text = Label::createWithTTF(TTFConfig("fonts/misaki_gothic.ttf", 20), "左上に移動する"); break;
	case RouteDirection::LowerLeft:  text = Label::createWithTTF(TTFConfig("fonts/misaki_gothic.ttf", 20), "左下に移動する"); break;
	case RouteDirection::UpperRight: text = Label::createWithTTF(TTFConfig("fonts/misaki_gothic.ttf", 20), "右上に移動する"); break;
	case RouteDirection::LowerRight: text = Label::createWithTTF(TTFConfig("fonts/misaki_gothic.ttf", 20), "右下に移動する"); break;
	}
	text->setScale(0.5f);
	text->setPositionY(20);
	arrow->addChild(text);

	dirtParticle = nullptr;
	ChangeState(routeState);

	return true;
}

void ChangeAreaArrow::ChangeState(RouteState routeState)
{
	arrow->setVisible(routeState == RouteState::Normal);

	if (dirtParticle)
	{
		dirtParticle->stopSystem();
		dirtParticle = nullptr;
	}

	switch (routeState)
	{
	case RouteState::Dirt:      CreateParticle("Particle/dirt_particle.plist"); break;
	case RouteState::HeavyDirt: CreateParticle("Particle/heavy_dirt_particle.plist"); break;
	case RouteState::BadDirt:   CreateParticle("Particle/bad_dirt_particle.plist"); break;
	case RouteState::DangerousDirt: CreateParticle("Particle/dangerous_dirt_particle.plist"); break;
	}
}

void ChangeAreaArrow::CreateParticle(const std::string& str)
{
	dirtParticle = ParticleSystemQuad::create(str);
	dirtParticle->setAutoRemoveOnFinish(true);
	dirtParticle->resetSystem();
	dirtParticle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	dirtParticle->setScale(0.25f);
	dirtParticle->setPosition(0, 0);
	this->addChild(dirtParticle);
}
