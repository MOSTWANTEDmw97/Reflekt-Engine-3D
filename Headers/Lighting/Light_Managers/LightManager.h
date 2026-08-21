#pragma once
#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H


#include <glad/glad.h>
#include <glm.hpp>
#include <vector>
#include "GLBuffers/SSBO.h"

struct LightData
{
    glm::ivec4 type;        // type in .x (0 = directional, 1 = point, 2 = spot)
    glm::vec4 position;     // xyz = position, w unused
    glm::vec4 direction;    // xyz = direction, w unused
    glm::vec4 color;        // rgb = color, a = intensity
    glm::vec4 attenuation;  // x = constant, y = linear, z = quadratic, w unused
    glm::vec4 cutoff;       // x = innerCutoff, y = outerCutoff, z/w unused
};

class LightManager
{
    public:

		std::vector<LightData> lights;

		LightManager();
		void Add(const LightData& light);
		void Clear();
		void Upload();
		void Bind();
		void Unbind();
		void DeleteBuffer();

	private:
		SSBO ssbo;
};

#endif // !LIGHTMANAGER_H
