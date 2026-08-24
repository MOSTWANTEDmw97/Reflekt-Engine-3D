#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<vector>
#include<Math/Transform.h>
#include<Component.h>

class GameObject
{
	public:
		Transform transform;
		std::vector<Component*> components;
		GameObject() = default;
		~GameObject()
        {
           // for (auto* c : components) delete c;
        }

        template<typename T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            T* comp = new T(std::forward<Args>(args)...);
            comp->gameObject = this;
            components.push_back(comp);
            return comp;
        }

        template<typename T>
        T* GetComponent()
        {
            for (Component* c : components)
            {
                if (auto* casted = dynamic_cast<T*>(c)) return casted;
            }
            return nullptr;
        }
};

#endif // !GAMEOBJECT_H
