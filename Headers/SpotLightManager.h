#pragma once
#ifndef SPOTLIGHTMANAGER_H
#define SPOTLIGHTMANAGER_H
#include<glm.hpp>
#include<vector>
#include"SSBO.h"

struct SpotLightData
{
    glm::vec3 position;  float cutoff;
    glm::vec3 direction; float outerCutoff;
    glm::vec3 ambient;   float constant;
    glm::vec3 diffuse;   float linear;
    glm::vec3 specular;  float quadratic;
};

class SpotLightManager
{
    public:

        std::vector<SpotLightData> lights;

        SpotLightManager(GLuint bindingPoint = 2);

        void Add(const SpotLightData& light);
        void Clear();
        void Upload();
        void Bind();
        void Unbind();
        void DeleteBuffer();

    private:
        SSBO ssbo;
};

#endif // !SPOTLIGHTMANAGER_H
