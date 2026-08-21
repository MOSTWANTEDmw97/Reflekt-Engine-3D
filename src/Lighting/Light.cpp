#include"Lighting/Light.h"

Light::Light(LightManager& mgr,
    const LightType type,
    Transform& transform,
    glm::vec3 direction,
    const glm::vec3& color,
    const float intensity,
    const glm::vec3& attenuation,
    const glm::vec2& cutoff)
    : manager(mgr), transform(transform)
{
    LightData light{};
    light.type = glm::ivec4(type, 0, 0, 0);
    light.position = glm::vec4(transform.position, 1.0f);
    light.direction = glm::vec4(direction, 0.0f);
    light.color = glm::vec4(color, intensity);
    light.attenuation = glm::vec4(attenuation, 0.0f);
    light.cutoff = glm::vec4(cutoff, 0.0f, 0.0f);

    manager.Add(light);
    index = (int)manager.lights.size() - 1;
    manager.Upload();
    manager.Bind();
}

void Light::SetPosition(const glm::vec3& pos)
{
    transform.position = pos;
    manager.lights[index].position = glm::vec4(pos, 1.0f);
    manager.Upload();
}

void Light::SetDirection(const glm::vec3& dir)
{
    manager.lights[index].direction = glm::vec4(dir, 0.0f);
    manager.Upload();
}

void Light::SetColor(const glm::vec3& col, float intensity)
{
    manager.lights[index].color = glm::vec4(col, intensity);
    manager.Upload();
}

void Light::SetAttenuation(float constant, float linear, float quadratic)
{
    manager.lights[index].attenuation = glm::vec4(constant, linear, quadratic, 0.0f);
    manager.Upload();
}

void Light::SetCutoff(float inner, float outer)
{
    manager.lights[index].cutoff = glm::vec4(inner, outer, 0.0f, 0.0f);
    manager.Upload();
}

void Light::SetAll(const glm::vec3& pos,
    const glm::vec3& dir,
    const glm::vec3& col,
    float intensity,
    const glm::vec3& attenuation,
    const glm::vec2& cutoff)
{
    transform.position = pos;
    manager.lights[index].position = glm::vec4(pos, 1.0f);
    manager.lights[index].direction = glm::vec4(dir, 0.0f);
    manager.lights[index].color = glm::vec4(col, intensity);
    manager.lights[index].attenuation = glm::vec4(attenuation, 0.0f);
    manager.lights[index].cutoff = glm::vec4(cutoff, 0.0f, 0.0f);

    manager.Upload();
}