#pragma once
#include "Effect.hpp"

class ObjectLayer;

class EffectFactory
{
private:
	EffectFactory() = default;
	~EffectFactory() = default;

	static EffectFactory* instance;

public:
	EffectFactory(const EffectFactory& obj) = delete;
	EffectFactory& operator=(const EffectFactory& obj) = delete;
	EffectFactory(EffectFactory&& obj) = delete;
	EffectFactory& operator=(EffectFactory&& obj) = delete;
		 
	static EffectFactory* getInstance();

	Effect* CreateEffect(ObjectLayer* objectLayer, EffectID effectID);
};
