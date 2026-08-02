#include"Lighting/Light_Managers/PointLightManager.h"

PointLightManager::PointLightManager(GLuint bindingPoint)
    : ssbo(bindingPoint)
{}

void PointLightManager::Add(PointLightData& light)
{
    lights.push_back(light);
}

void PointLightManager::Clear()
{
    lights.clear();
}

void PointLightManager::Upload()
{
    if (!lights.empty())
    {
        ssbo.Upload(
            lights.empty() ? nullptr : lights.data(),
            lights.size() * sizeof(PointLightData),
            GL_DYNAMIC_DRAW
        );
    }
}

void PointLightManager::Bind()
{
    ssbo.Bind();
}

void PointLightManager::Unbind()
{
    ssbo.Unbind();
}

void PointLightManager::DeleteBuffer()
{
    ssbo.Delete();
}