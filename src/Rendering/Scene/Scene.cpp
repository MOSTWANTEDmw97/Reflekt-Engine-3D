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

void Scene::DrawGeometry(float width, float height, Shader* overrideShader)
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

    //std::cout << "Count: " << pipeline.ReturnOpaqueQueueCount() << std::endl;
    pipeline.DrawOpaque(overrideShader);
    if (activeCamera->skybox)
    {

        //activeCamera->BindToShader(*activeCamera->skybox->shader, glm::mat4(1.0f), 1920.0f / 1080.0f);
        //activeCamera->skybox->Draw();
    }
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