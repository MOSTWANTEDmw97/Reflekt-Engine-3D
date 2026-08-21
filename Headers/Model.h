#pragma once
#ifndef MODEL_H
#define MODEL_H

#include<vector>
#include<string>
#include"Graphics/Shader.h"
#include"Mesh.h"
#include"Math/Transform.h"
#include"UI/IMGUI/Inspector.h"

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

		void Draw();
		void DrawInstanced(const std::vector<glm::mat4>& transforms);
		void Delete();

		std::vector<Mesh>& GetMeshes() { return meshes; }

		//std::string GetName() const { return "Model"; }
		void DrawInspector()
		{
			DrawTransformInspector(transform);
		}

		private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& nodeTransform);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
			const std::string& typeName,
			const aiScene* scene,
			const std::string& directory);
		Texture LoadEmbeddedTexture(const aiTexture* texture, const std::string& typeName);

		glm::mat4 ConvertToGLM(const aiMatrix4x4& m);
		
};

#endif // !MODEL_H
