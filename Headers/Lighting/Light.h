#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include"Lighting/Light_Managers/LightManager.h"
#include"Math/Transform.h"

enum LightType
{
    directionalLight = 0,
    pointLight = 1,
    spotLight = 2
};

class Light
{   
    public:
        Transform transform;
        Light(LightManager& mgr,
            LightType type, // 0 = directional, 1 = point, 2 = spot
            Transform& transform,
            glm::vec3 direction,
            const glm::vec3& color,
            float intensity,
            const glm::vec3& attenuation = glm::vec3(1.0f, 0.0f, 0.0f),
            const glm::vec2& cutoff = glm::vec2(0.0f, 0.0f));

        void SetPosition(const glm::vec3& pos);
        void SetDirection(const glm::vec3& dir);
        void SetColor(const glm::vec3& col, float intensity);
        void SetAttenuation(float constant, float linear, float quadratic);
        void SetCutoff(float inner, float outer);

        void SetAll(const glm::vec3& pos,
            const glm::vec3& dir,
            const glm::vec3& col,
            float intensity,
            const glm::vec3& attenuation,
            const glm::vec2& cutoff);

private:
    LightManager& manager;
    int index;
};

#endif // !LIGHT_H
