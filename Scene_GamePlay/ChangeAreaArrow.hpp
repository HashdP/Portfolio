#pragma once
#include "cocos2d.h"

enum RouteDirection;
enum class RouteState;

class ChangeAreaArrow : public cocos2d::Node
{
private:
	ChangeAreaArrow() = default;

	RouteDirection routeDirection;

	cocos2d::Node* arrow;

	cocos2d::ParticleSystemQuad* dirtParticle;

	void CreateParticle(const std::string& str);

public:
	~ChangeAreaArrow() = default;
	static ChangeAreaArrow* create(RouteDirection RouteDirection, RouteState routeState);
	virtual bool init(RouteDirection RouteDirection, RouteState routeState);

	/*
	arrow��dirtParticle��routeState�̂��̂ɕύX����
	*/
	void ChangeState(RouteState routeState);
};
