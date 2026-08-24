#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include<vector>
#include"Shader.h"
#include"Texture.h"

enum SurfaceType{Opaque, Transparent};
enum BlendMode{Alpha, PreMultiply, Additive, Multiply};
enum CullMode{Back, Front, None};


class Material
{
public:
	Shader* shaderRef;
	std::vector<Texture> textures;
	float shininess;
	SurfaceType surfaceType;
	BlendMode blendMode;
	CullMode cullMode;
	int renderQueue;
	bool castShadow;
	bool reciveShadow;

	//Material() : shininess(32.0f) {} // Default
	Material(Shader* shader, const std::vector<Texture>& textures, float shininess = 32.0f,
		SurfaceType surfaceType = SurfaceType::Opaque, 
		BlendMode blendMode = BlendMode::Alpha,
		CullMode cullMode = CullMode::Back);
	
	void Apply(Shader* overrideShader = nullptr);
	
};
#endif // !MATERIAL_H
