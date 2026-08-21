#include<Lighting/Light_Managers/LightManager.h>

LightManager::LightManager()
	:ssbo(0) {}

void LightManager::Add(const LightData& light)
{
    lights.push_back(light);
}

void LightManager::Clear()
{
    lights.clear();
}

void LightManager::Upload()
{
    if (!lights.empty())
    {
        ssbo.Upload(
            lights.empty() ? nullptr : lights.data(),
            lights.size() * sizeof(LightData),
            GL_DYNAMIC_DRAW);
    }
}

void LightManager::Bind()
{
    ssbo.Bind();
}

void LightManager::Unbind()
{
    ssbo.Unbind();
}

void LightManager::DeleteBuffer()
{
    ssbo.Delete();
}