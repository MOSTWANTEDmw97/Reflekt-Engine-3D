#pragma once
#ifndef POINTLIGHTMANAGER_H
#define POINTLIGHTMANAGER_H

#include<glm.hpp>
#include<vector>
#include"GLBuffers/SSBO.h"

struct PointLightData
{
    glm::vec4 position;  
    glm::vec4 ambient_And_Constant;   
    glm::vec4 diffuse_And_Linear;  
    glm::vec4 specular_And_Quadratic; 
    
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
