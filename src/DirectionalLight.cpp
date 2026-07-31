#include"DirectionalLight.h"

DirectionalLight::DirectionalLight(DirectionalLightManager& mgr,
    const glm::vec3& direction,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular)
    :manager(mgr)
{
    DirectionalLightData light
    {
        direction, 1.0f,
        ambient, 1.0f,
        diffuse, 1.0f,
        specular, 1.0f
    };
    manager.Add(light);
    index = (int)manager.lights.size() - 1;
    manager.Upload();
    manager.Bind();
}

void DirectionalLight::SetDirection(const glm::vec3& direction)
{
    manager.lights[index].direction = direction;
    manager.Upload();
}

void DirectionalLight::SetAmbient(const glm::vec3& ambient)
{
    manager.lights[index].ambient = ambient;
    manager.Upload();
}

void DirectionalLight::SetDiffuse(const glm::vec3& diffuse)
{
    manager.lights[index].diffuse = diffuse;
    manager.Upload();
}

void DirectionalLight::SetSpecular(const glm::vec3& specular)
{
    manager.lights[index].specular = specular;
    manager.Upload();
}

void DirectionalLight::SetAll(const glm::vec3& direction,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular)
{

    manager.lights[index].direction = direction;

    manager.lights[index].ambient = ambient;
    manager.lights[index].diffuse = diffuse;
    manager.lights[index].specular = specular;

    manager.Upload();
}

