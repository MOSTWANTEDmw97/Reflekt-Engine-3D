#include"SpotLight.h"

SpotLight::SpotLight(SpotLightManager& mgr,
    const glm::vec3& position,
    const glm::vec3& direction,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular,
    const float cutoff, const float outerCutoff,
    const float constant, const float linear, const float quadratic)
    :manager(mgr)
{
    SpotLightData light
    {
        position, cutoff,
        direction, outerCutoff,
        ambient, constant,
        specular, linear,
        diffuse, quadratic
    };

    manager.Add(light);
    index = (int)manager.lights.size() - 1;
    manager.Upload();
    manager.Bind();
}

void SpotLight::SetPosition(const glm::vec3& position)
{
    manager.lights[index].position = position;
    manager.Upload();
}
void SpotLight::SetDirection(const glm::vec3& direction)
{
    manager.lights[index].direction = direction;
    manager.Upload();
}
void SpotLight::SetAmbient(const glm::vec3& ambient)
{
    manager.lights[index].ambient = ambient;
    manager.Upload();
}
void SpotLight::SetDiffuse(const glm::vec3& diffuse)
{
    manager.lights[index].diffuse = diffuse;
    manager.Upload();
}
void SpotLight::SetSpecular(const glm::vec3& specular)
{
    manager.lights[index].specular = specular;
    manager.Upload();
}

void SpotLight::SetCutoff(const float cutoff, const float outerCutoff)
{
    manager.lights[index].cutoff = cutoff;
    manager.lights[index].outerCutoff = outerCutoff;
    manager.Upload();
}

void SpotLight::SetAttenuation(const float constant, const float linear, const float quadratic)
{
    manager.lights[index].constant = constant;
    manager.lights[index].linear = linear;
    manager.lights[index].quadratic = quadratic;
    manager.Upload();
}

void SpotLight::SetAll(const glm::vec3& position,
    const glm::vec3& direction,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular,
    const float cutoff, const float outerCutoff,
    const float constant, const float linear, const float quadratic)
{
    manager.lights[index].position = position;
    manager.lights[index].direction = direction;

    manager.lights[index].ambient = ambient;
    manager.lights[index].diffuse = diffuse;
    manager.lights[index].specular = specular;

    manager.lights[index].cutoff = cutoff;
    manager.lights[index].outerCutoff = outerCutoff;

    manager.lights[index].constant = constant;
    manager.lights[index].linear = linear;
    manager.lights[index].quadratic = quadratic;

    manager.Upload();
}