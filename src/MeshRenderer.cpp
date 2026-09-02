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

		Shader* defaultShader = new Shader("Default_Shaders/geometryPass.shader");
		material = new Material(defaultShader);
		material->SetFloat("material.shininess", 32.0f);

		material = new Material(defaultShader);
		material->SetFloat("material.shininess", 32.0f);


		//PlaceHolder for now 
		int slot = 0;
		for (Mesh& mesh : model->GetMeshes())
		{
			for (Texture& tex : mesh.GetTextures())
			{
				material->SetTexture("material.diffuse", &tex, slot);
				slot++;
			}
		}

	}
		
	

	void MeshRenderer::Draw(Shader* overrideShader)
	{

		if (!model || !material) return;

		Shader* activeShader = overrideShader ? overrideShader : material->shaderRef;

		material->Apply(activeShader);

		//Applying here cuz if i dont transparent breaks
		//And idfk why and IM TOO TIRED TO FIX TS
		activeShader->SetMat4("model", gameObject->transform.GetModelMatrix());
		
		if (isInstanced)
		{
			model->DrawInstanced(transforms);
		}
		else
		{//
			model->Draw();
		}

		// Reset state if transparent
		if (material->surfaceType == SurfaceType::Transparent)
		{
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
		}

	}
