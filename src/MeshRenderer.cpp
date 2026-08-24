	#include "MeshRenderer.h"
	#include<string>

	MeshRenderer::MeshRenderer(Model& m, Material& mat, bool instanced, const std::vector<glm::mat4>& transforms)
		: model(&m),
		isInstanced(instanced),
		transforms(transforms),
		material(&mat)
	{}
	

	MeshRenderer::MeshRenderer(Model& m, bool instanced, const std::vector<glm::mat4>& transforms)
		: model(&m),
		isInstanced(instanced),
		transforms(transforms)
	{

		std::vector<Texture> allTextures;
		for (Mesh& mesh : model->GetMeshes())
		{
			const auto& texs = mesh.GetTextures();
			allTextures.insert(allTextures.end(), texs.begin(), texs.end());
		}
		//Shader defaultShader("Default_Shaders/default_lit.shader");
		Shader* defaultShader = new Shader("Default_Shaders/geometryPass.shader");
		//Material mat(&defaultShader, allTextures, 32.0f);
		material = new Material(defaultShader, allTextures, 32.0f) ;
		//material = Material(&defaultShader, allTextures, 32.0f);
		// Build material with embedded textures
		//std::cout << "model" << std::endl;
	}

	void MeshRenderer::Draw(Shader* overrideShader)
	{

		if (!model || !material) return;

		Shader* activeShader = overrideShader ? overrideShader : material->shaderRef;

		material->Apply(activeShader);
		//activeShader->UsePass(renderPassName);
		activeShader->SetMat4("model", gameObject->transform.GetModelMatrix());
		if (isInstanced)
		{
			model->DrawInstanced(transforms);
		}
		else
		{
			model->Draw();
		}

		//std::cout << "Drawing object at: "
			//<< (gameObject->transform.position.x) << gameObject->transform.position.y<<gameObject->transform.position.z<< std::endl;

	}
