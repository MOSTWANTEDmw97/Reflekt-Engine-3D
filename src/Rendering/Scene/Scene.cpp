#include"Rendering/Scene/Scene.h"

void Scene::AddGameObject(GameObject& gameObject)
{
    gameObjects.push_back(gameObject);
}

void Scene::SetCamera(Camera* cam)
{
    activeCamera = cam;
    pipeline.SetCamera(cam);
}

void Scene::CollectGeometry(float width, float height, Shader* overrideShader)
{
    pipeline.opaqueQueue.clear();
    pipeline.transparentQueue.clear();
    for (auto& obj : gameObjects)
    {
        if (auto renderer = obj.GetComponent<MeshRenderer>())
        {
            Shader* activeShader = overrideShader ? overrideShader : renderer->GetMaterial().shaderRef;
            activeCamera->BindToShader(*activeShader, glm::mat4(1.0f), width / height);
            pipeline.AddMeshRenderer(*renderer);
        }
    }
}

void Scene::DrawAllGeometry(float width, float height, Shader* overrideShader)
{
    CollectGeometry(width, height, overrideShader);
    pipeline.DrawAll(overrideShader);
}

void Scene::DrawOpaqueGeometry(float width, float height, Shader* overrideShader)
{
    CollectGeometry(width, height, overrideShader);
    pipeline.DrawOpaque(overrideShader);
}

void Scene::DrawTranspaentGeometry(float width, float height, Shader* overrideShader)
{

    CollectGeometry(width, height, overrideShader);
    pipeline.DrawTransparent(overrideShader);
}

void Scene::UploadLights(Shader& lightingShader)
{
    for (auto obj : gameObjects)
    {
        if (auto lightComp = obj.GetComponent<LightComponent>())
            lightComp->light.UploadToShader(lightingShader);
    }
}