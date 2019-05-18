#include "Effect/Effect_PunchHit.hpp"

USING_NS_CC;

bool Effect_PunchHit::init()
{
	if (!Effect::init(1.0f))
		return false;

	ParticleSystemQuad* particle = ParticleSystemQuad::create("Particle/hit_punch_particle.plist");
	particle->setAutoRemoveOnFinish(true);
	particle->resetSystem();
	particle->setScale(0.2f);
	particle->setPosition(Vec2::ZERO);
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	this->addChild(particle);

	return true;
}
