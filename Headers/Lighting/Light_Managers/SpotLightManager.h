#pragma once
#ifndef SPOTLIGHTMANAGER_H
#define SPOTLIGHTMANAGER_H
#include<glm.hpp>
#include<vector>
#include"GLBuffers/SSBO.h"

struct SpotLightData
{
    glm::vec4 position_And_Cutoff;  
    glm::vec4 direction_And_OuterCutoff; 
    glm::vec4 ambient_And_Constant;  
    glm::vec4 diffuse_And_Linear;
    glm::vec4 specular_And_Quadratic;  
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
