#pragma once
#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include"GameObject.h"
#include "Camera.h"
#include "Rendering/RenderPipeline.h"
#include"MeshRenderer.h"
#include"LightComponent.h"
#include"CameraComponent.h"


class Scene
{
public:
    RenderPipeline pipeline;
    std::vector<GameObject> gameObjects;
    Camera* activeCamera = nullptr;

    void AddGameObject(GameObject& obj);
    void SetCamera(Camera* cam);
    void CollectGeometry(float width, float height, Shader* overrideShader = nullptr);
    void DrawAllGeometry(float width, float height, Shader* overrideShader = nullptr);
    void DrawOpaqueGeometry(float width, float height, Shader* overrideShader = nullptr);
    void DrawTransparentGeometry(float width, float height, Shader* overrideShader = nullptr);
    void UploadLights(Shader& lightingShader);
};

#endif
