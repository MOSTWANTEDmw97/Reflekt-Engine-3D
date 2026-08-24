#pragma once
#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include"Component.h"
#include"Lighting/Light.h"

class LightComponent : public Component
{
	public:
		DiffLight light;
		LightComponent(const DiffLight& l) : light(l) {}
};

#endif // !LIGHTCOMPONENT_H
