#pragma once
#ifndef DIRECTIONALLIGHTMANAGER_H
#define DIRECTIONALLIGHTMANAGER_H

#include<glad/glad.h>
#include<glm.hpp>
#include<vector>
#include"GLBuffers/SSBO.h"

struct DirectionalLightData
{
	glm::vec4 direction; 
	glm::vec4 ambient;   
	glm::vec4 diffuse;   
	glm::vec4 specular;  
};


class DirectionalLightManager
{
public:

	std::vector<DirectionalLightData> lights;

	DirectionalLightManager(GLuint bindingPoint = 0);
	void Add(const DirectionalLightData& light);
	void Clear();
	void Upload();
	void Bind();
	void Unbind();
	void DeleteBuffer();

private:
	SSBO ssbo;
};

#endif // !DIRECTIONALLIGHT_H
#pragma once
