#pragma once
#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include"Component.h"
#include"Lighting/Light.h"

class LightComponent : public Component
{
	public:
		Light light;	
		LightComponent(const Light& l) : light(l) {}
};

#endif // !LIGHTCOMPONENT_H
