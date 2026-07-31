#pragma once
#ifndef POINTLIGHTMANAGER_H
#define POINTLIGHTMANAGER_H

#include<glm.hpp>
#include<vector>
#include"SSBO.h"

struct PointLightData
{
    glm::vec3 position;  float constant;
    glm::vec3 ambient;   float linear;
    glm::vec3 diffuse;   float quadratic;
    glm::vec3 specular; float pad5;
    
};

class PointLightManager
{
    public:

        std::vector<PointLightData> lights;

        PointLightManager(GLuint bindingPoint = 1);

        void Add(PointLightData& light);
        void Clear();
        void Upload();
        void Bind();
        void Unbind();
        void DeleteBuffer();

    private:
        SSBO ssbo;
};

#endif // !POINTLIGHTMANAGER_H
