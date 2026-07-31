#pragma once
#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include"DirectionalLightManager.h"

class DirectionalLight
{
public:
    DirectionalLight(DirectionalLightManager& mgr,
        const glm::vec3& direction,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular);


    void SetDirection(const glm::vec3& direction);
    void SetAmbient(const glm::vec3& ambient);
    void SetDiffuse(const glm::vec3& diffuse);
    void SetSpecular(const glm::vec3& specular);

    void SetAll(const glm::vec3& direction,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular);

private:
    DirectionalLightManager& manager;
    int index;
};

#endif // !DIRECTIONALLIGHT_H
