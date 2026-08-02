#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include<vector>
#include"Shader.h"
#include"Texture.h"

class Material
{
public:
	std::vector<Texture> textures;
	float shininess;

	Material() : shininess(32.0f) {} // Default
	Material(const std::vector<Texture>& textures, float shininess);
	
	void Apply(Shader& shader);
	
};
#endif // !MATERIAL_H
