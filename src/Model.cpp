#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<iostream>
#include<filesystem>

#include"Model.h"
#include"Graphics/ShaderManager.h"

//TODO:
//Support for multiple meshes and materials in one models 

Model::Model(const std::vector<Mesh>& meshList, const Transform& initialTransform)
	: meshes(meshList), transform(initialTransform) {}

Model::Model(const std::string& path, const Transform& initialTransform)
	:transform(initialTransform)
{
	LoadModel(path);
}

void Model::Draw()
{
	glm::mat4 modelMatrix = transform.GetModelMatrix();
	for (Mesh& mesh : meshes)
	{
		mesh.GetMaterial().shaderRef->SetMat4("model", modelMatrix);
		mesh.Draw();
	}
}

void Model::DrawInstanced(const std::vector<glm::mat4>& transforms)
{
	glm::mat4 modelMatrix = transform.GetModelMatrix();
	for (Mesh& mesh : meshes)
	{
		//mesh.SetupInstanceBuffer(transforms);
		mesh.GetMaterial().shaderRef->SetMat4("model", modelMatrix);
		mesh.DrawInstanced(static_cast<GLsizei>(transforms.size()));
		

	}
}

void Model::Delete()
{
	for (Mesh& mesh : meshes)
	{
		mesh.Delete();
	}
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, 
		aiProcess_Triangulate | aiProcess_CalcTangentSpace |
		aiProcess_FlipWindingOrder |
		aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices | 
		aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality);

	if (scene)
	{
		//std::cout << "Scene has " << scene->mNumTextures << " embedded textures." << std::endl;
	}

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
		return;
	}

	glm::mat4 rootTransform = ConvertToGLM(scene->mRootNode->mTransformation);

	/*
	std::cout << "Root transform:" << std::endl;
	std::cout << rootTransform[0][0] << ", " << rootTransform[0][1] << ", " << rootTransform[0][2] << ", " << rootTransform[0][3] << std::endl;
	std::cout << rootTransform[1][0] << ", " << rootTransform[1][1] << ", " << rootTransform[1][2] << ", " << rootTransform[1][3] << std::endl;
	std::cout << rootTransform[2][0] << ", " << rootTransform[2][1] << ", " << rootTransform[2][2] << ", " << rootTransform[2][3] << std::endl;
	std::cout << rootTransform[3][0] << ", " << rootTransform[3][1] << ", " << rootTransform[3][2] << ", " << rootTransform[3][3] << std::endl;
	*/

	ProcessNode(scene->mRootNode, scene, glm::mat4(1.0f));
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform)
{
	glm::mat4 nodeTransform = parentTransform * ConvertToGLM(node->mTransformation);
	//std::cout << "processing mesh..." << std::endl;
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(std::move(ProcessMesh(mesh, scene, nodeTransform)));

	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, nodeTransform);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& nodeTransform)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;


	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces * 3); // most faces are triangles

	// Verts
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec4 pos(mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z,
			1.0f);
		
		//pos = nodeTransform * pos;
		vertex.position = glm::vec3(pos);

		//vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		//vertex.normal = mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0.0f);
		if (mesh->HasNormals())
		{
			glm::vec3 normal(mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z);
			vertex.normal = glm::mat3(glm::transpose(glm::inverse(nodeTransform))) * normal;
		}


		vertex.color = glm::vec3(1.0f); // default white
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f);
		}
		vertices.push_back(vertex);
		/*
		std::cout << "Vertex " << i << ": Position(" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z
			<< "), Normal(" << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z
			<< "), TexCoords(" << vertex.texCoords.x << ", " << vertex.texCoords.y << ")" << std::endl;
		*/	
	}

	// Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}


	// Materials
	std::vector<Texture> textures;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		auto diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse", scene, directory);
		auto specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular", scene, directory);

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	//Shader defaultShader("Default_Shaders/Default.shader");
	Shader& defaultShader = ShaderManager::Exists("default")
		? ShaderManager::Get("default") : 
		ShaderManager::Get("light");


	Material material(&defaultShader, textures, 128.0f);


	return Mesh(vertices, indices, material, GL_STATIC_DRAW);
}


std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat,
	aiTextureType type,
	const std::string& typeName,
	const aiScene* scene,
	const std::string& directory)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		//std::cout << "Loading texture: " << str.C_Str() << std::endl;

		if (str.C_Str()[0] == '*')
		{
			// Embedded texture referenced properly
			int texIndex = atoi(str.C_Str() + 1);
			const aiTexture* aiTex = scene->mTextures[texIndex];
			//std::cout << "Loading embedded texture #" << texIndex << " (" << typeName << ")" << std::endl;
			textures.push_back(LoadEmbeddedTexture(aiTex, typeName));
		}
		else
		{
			// External texture OR embedded but referenced by filename
			std::string texName = str.C_Str();

			// If scene has embedded textures but material string is a filename,
			// load them directly instead of failing
			if (scene->mNumTextures > 0)
			{
				for (unsigned int j = 0; j < scene->mNumTextures; j++)
				{
					const aiTexture* aiTex = scene->mTextures[j];
					/*
					std::cout << "Material referenced external file, but scene has embedded texture #"
						<< j << ". Loading embedded instead (" << typeName << ")" << std::endl;
					*/
					textures.push_back(LoadEmbeddedTexture(aiTex, typeName));
				}
			}
			else
			{
				// External texture path resolution
				std::filesystem::path texPath = std::filesystem::path(directory) / texName;
				std::string fullPath = texPath.string();

				if (std::filesystem::exists(fullPath))
				{
					//std::cout << "Resolved texture path: " << fullPath << std::endl;
					textures.emplace_back(fullPath.c_str(), typeName);
				}
				else
				{
					/*
					std::cout << "Texture not found at: " << fullPath
						<< ". Trying basename in model directory." << std::endl;
					*/
					std::string basename = std::filesystem::path(texName).filename().string();
					std::filesystem::path basePath = std::filesystem::path(directory) / basename;
					std::string baseFullPath = basePath.string();

					//std::cout << "Resolved texture path: " << baseFullPath << std::endl;
					textures.emplace_back(baseFullPath.c_str(), typeName);
				}
			}
		}
	}

	return textures;
}

Texture Model::LoadEmbeddedTexture(const aiTexture* aiTex, const std::string& typeName)
{
	return Texture(aiTex, typeName);
}

glm::mat4 Model::ConvertToGLM(const aiMatrix4x4& m)
{
	return glm::mat4(
		m.a1, m.b1, m.c1, m.d1,
		m.a2, m.b2, m.c2, m.d2,
		m.a3, m.b3, m.c3, m.d3,
		m.a4, m.b4, m.c4, m.d4
	);
}