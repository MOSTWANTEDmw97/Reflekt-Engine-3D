#include"Lighting/DirectionalLight.h"

DirectionalLight::DirectionalLight(DirectionalLightManager& mgr,
	Transform& transform,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular)
    :manager(mgr)
{
    DirectionalLightData light
    {
		glm::vec4(transform.Forward, 1.0f),
		glm::vec4(ambient, 1.0f),
		glm::vec4(diffuse, 1.0f),
		glm::vec4(specular, 1.0f)
    };
    manager.Add(light);
    index = (int)manager.lights.size() - 1;
    manager.Upload();
    manager.Bind();
}

void DirectionalLight::SetDirection(const glm::vec3& direction)
{
	transform.Forward = glm::normalize(direction);
    manager.lights[index].direction = glm::vec4(direction, 1.0f);
    manager.Upload();
}

void DirectionalLight::SetAmbient(const glm::vec3& ambient)
{
    manager.lights[index].ambient = glm::vec4(ambient, 1.0f);
    manager.Upload();
}

void DirectionalLight::SetDiffuse(const glm::vec3& diffuse)
{
    manager.lights[index].diffuse = glm::vec4(diffuse, 1.0f);
    manager.Upload();
}

void DirectionalLight::SetSpecular(const glm::vec3& specular)
{
    manager.lights[index].specular = glm::vec4(specular, 1.0f);
    manager.Upload();
}

void DirectionalLight::SetAll(const glm::vec3& direction,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular)
{
	transform.Forward = glm::normalize(direction);
    manager.lights[index].direction = glm::vec4(direction, 1.0f)        ;

    manager.lights[index].ambient = glm::vec4(ambient, 1.0f);
    manager.lights[index].diffuse = glm::vec4(diffuse, 1.0f);
    manager.lights[index].specular = glm::vec4(specular, 1.0f);

    manager.Upload();
}

