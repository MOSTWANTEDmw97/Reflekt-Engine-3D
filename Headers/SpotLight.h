#pragma once
#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include"SpotLightManager.h"

class SpotLight
{

public:
    SpotLight(SpotLightManager& mgr,
        const glm::vec3& position,
        const glm::vec3& direction,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        const float cutoff, const float outerCutoff,
        const float constant, const float linear, const float quadratic);


    void SetPosition(const glm::vec3& pos);
    void SetDirection(const glm::vec3& dir);
    void SetAmbient(const glm::vec3& amb);
    void SetDiffuse(const glm::vec3& diff);
    void SetSpecular(const glm::vec3& spec);
    void SetCutoff(const float cutoff, const float outerCutoff);
    void SetAttenuation(const float constant, const float linear, const float quadratic);

    void SetAll(const glm::vec3& position,
        const glm::vec3& direction,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        const float cutoff, const float outerCutoff,
        const float constant, const float linear, const float quadratic);

private:
    SpotLightManager& manager;
    int index;
};


#endif // !SPOTLIGHT_H
