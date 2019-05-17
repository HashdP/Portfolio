#include "Effect_DirtShotHit.hpp"
#include "Light.hpp"

USING_NS_CC;

bool Effect_DirtShotHit::init()
{
	if (!Effect::init(1.0f))
		return false;

	Light* light = Light::create("Images/lightmask.png", 3);
	light->setColor(Color3B(255, 200, 100));
	light->setScale(0.1f);
	light->runAction(
		Sequence::create(
			FadeOut::create(0.1f),
			CallFuncN::create([light](Ref* sender) { light->removeFromParent(); }),
			NULL
		)
	);
	this->addChild(light);

	ParticleSystemQuad* particle = ParticleSystemQuad::create("Particle/dirtshot_hit_particle.plist");
	particle->setAutoRemoveOnFinish(true);
	particle->resetSystem();
	particle->setScale(0.4f);
	particle->setPosition(Vec2::ZERO);
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	this->addChild(particle);

	return true;
}
