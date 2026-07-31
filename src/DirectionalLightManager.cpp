#include"DirectionalLightManager.h"
#include<iostream>

DirectionalLightManager::DirectionalLightManager(GLuint bindingPoint)
	:ssbo(bindingPoint)
{}

void DirectionalLightManager::Add(const DirectionalLightData& light)
{
	lights.push_back(light);
}
void DirectionalLightManager::Clear()
{
	lights.clear();
}
void DirectionalLightManager::Upload()
{
    if (!lights.empty())
    {
        ssbo.Upload(
            lights.empty() ? nullptr : lights.data(),
            lights.size() * sizeof(DirectionalLightData),
            GL_DYNAMIC_DRAW);
    }
}

void DirectionalLightManager::Bind()
{
    ssbo.Bind();
}

void DirectionalLightManager::Unbind()
{
    ssbo.Unbind();
}

void DirectionalLightManager::DeleteBuffer()
{
    ssbo.Delete();
}