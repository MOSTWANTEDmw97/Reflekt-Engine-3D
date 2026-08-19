#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include<vector>
#include"Shader.h"
#include"Texture.h"

enum BlendMode{Opaque, Transparent};

class Material
{
public:
	Shader* shaderRef;
	std::vector<Texture> textures;
	float shininess;
	BlendMode blendMode;

	Material() : shininess(32.0f) {} // Default
	Material(Shader* shader, const std::vector<Texture>& textures, float shininess, BlendMode blendMode = Opaque);
	
	void Apply();
	
};
#endif // !MATERIAL_H
