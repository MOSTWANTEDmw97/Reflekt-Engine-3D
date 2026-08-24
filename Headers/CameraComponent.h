#pragma once
#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include"Component.h"
#include"Camera.h"

class CameraComponent : public Component
{
public:
    Camera camera;

    CameraComponent(const Camera& cam) : camera(cam) {}
};

#endif // !CAMERACOMPONENT_H
