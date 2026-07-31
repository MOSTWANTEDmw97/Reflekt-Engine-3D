#include"SpotLightManager.h"

SpotLightManager::SpotLightManager(GLuint bindingPoint)
    : ssbo(bindingPoint)
{
}

void SpotLightManager::Add(const SpotLightData& light)
{
    lights.push_back(light);
}

void SpotLightManager::Clear()
{
    lights.clear();
}

void SpotLightManager::Upload()
{
    if (!lights.empty())
    {
        ssbo.Upload(
            lights.empty() ? nullptr : lights.data(),
            lights.size() * sizeof(SpotLightData),
            GL_DYNAMIC_DRAW
        );
    }
}

void SpotLightManager::Bind()
{
    ssbo.Bind();
}

void SpotLightManager::Unbind()
{
    ssbo.Unbind();
}

void SpotLightManager::DeleteBuffer()
{
    ssbo.Delete();
}