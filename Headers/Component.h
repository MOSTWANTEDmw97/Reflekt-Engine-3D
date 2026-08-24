#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

//#include"GameObject.h"

class GameObject;

class Component
{
	public:
		GameObject* gameObject = nullptr; //Parent reference
		virtual ~Component() = default;

};
#endif // !COMPONENT_H
