#pragma once
#ifndef MODEL_H
#define MODEL_H

#include<vector>
#include<string>
#include"Shader.h"
#include"Mesh.h"
#include"Math/Transform.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

struct aiNode;
struct aiScene;
struct aiMesh;

class Model
{
	public:
		std::vector<Mesh> meshes;
		Transform transform;
		std::string directory;

		Model() = default;
		Model(const std::vector<Mesh>& meshList, const Transform& initialTransform = Transform());
		Model(const std::string& path, const Transform& initialTransform = Transform());

		void Draw(Shader& shader);
		void Delete();

	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
			const std::string& typeName,
			const aiScene* scene,
			const std::string& directory);
		Texture LoadEmbeddedTexture(const aiTexture* texture, const std::string& typeName);
		//glm::mat4 ConvertToGLM(const aiMatrix4x4& m);
};

#endif // !MODEL_H
