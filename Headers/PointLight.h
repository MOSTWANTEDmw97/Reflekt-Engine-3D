#pragma once
#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include"PointLightManager.h"

class PointLight
{
	public:
        PointLight(PointLightManager& mgr,
            const glm::vec3& position,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular,
            const float constant, const float linear, const float quadratic);

        
        void SetPosition(const glm::vec3& pos);
        void SetAmbient(const glm::vec3& amb);
        void SetDiffuse(const glm::vec3& diff);
        void SetSpecular(const glm::vec3& spec);
        void SetAttenuation(const float constant, const float linear, const float quadratic);

        void SetAll(const glm::vec3& position,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular,
            const float constant, const float linear, const float quadratic);

    private:
        PointLightManager& manager;
        int index;
};

#endif // !POINTLIGHT_H
