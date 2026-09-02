	#pragma once
	#ifndef MATERIAL_H
	#define MATERIAL_H


	#include <unordered_map>
	#include <string>
	#include<vector>
	#include"Shader.h"
	#include"Texture.h"

	enum SurfaceType{Opaque, Transparent};
	enum BlendMode{Alpha, PreMultiply, Additive, Multiply};
	enum CullMode{Back, Front, None};

	struct MaterialTextures
	{
		Texture* tex;
		int slot;
	};

	class Material
	{
		public:
			//std::vector<Texture> textures;
			//float shininess;

			Shader* shaderRef;
			SurfaceType surfaceType;
			BlendMode blendMode;
			CullMode cullMode;
			int renderQueue;

			//Material() : shininess(32.0f) {} // Default
			/*Material(Shader* shader, const std::vector<Texture>& textures, float shininess = 32.0f,
				SurfaceType surfaceType = SurfaceType::Opaque, 
				BlendMode blendMode = BlendMode::PreMultiply,
				CullMode cullMode = CullMode::Back);*/
		
			Material(Shader* shader);

			//Shader property setters
			void SetFloat(const std::string& name, float v);
			void SetInt(const std::string& name, int v);
			void SetBool(const std::string& name, bool v);
			void SetVec2(const std::string& name, const glm::vec2& v);
			void SetVec3(const std::string& name, const glm::vec3& v);
			void SetVec4(const std::string& name, const glm::vec4& v);
			void SetMat4(const std::string& name, const glm::mat4& v);
			void SetTexture(const std::string& name,Texture* tex, int slot = 0);
			void SetCubeMapTexture(const std::string& name, Texture* tex, int slot = 0);

			void Apply(Shader* overrideShader = nullptr);
	
		private:

			//Shader material properties
			std::unordered_map < std::string, bool> boolProperties;
			std::unordered_map<std::string, int> intProperties;
			std::unordered_map<std::string, float> floatProperties;
			std::unordered_map < std::string, glm::vec2> vec2Properties;
			std::unordered_map < std::string, glm::vec3> vec3Properties;
			std::unordered_map < std::string, glm::vec4> vec4Properties;
			std::unordered_map < std::string, glm::mat4> mat4Properties;
			std::unordered_map < std::string, MaterialTextures> textureProperties;
			std::unordered_map < std::string, MaterialTextures> cubemapTextureProperties;

	};
	#endif // !MATERIAL_H
