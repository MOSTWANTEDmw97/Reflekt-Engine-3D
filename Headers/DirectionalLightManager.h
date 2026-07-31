#pragma once
#ifndef DIRECTIONALLIGHTMANAGER_H
#define DIRECTIONALLIGHTMANAGER_H

#include<glad/glad.h>
#include<glm.hpp>
#include<vector>
#include"SSBO.h"

struct DirectionalLightData
{
	glm::vec3 direction; float pad1;
	glm::vec3 ambient;   float pad2;
	glm::vec3 diffuse;   float pad3;
	glm::vec3 specular;  float pad4;
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
