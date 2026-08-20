#include"Graphics/Material.h"

Material::Material(Shader* shader, const std::vector<Texture>& textures, float shininess, SurfaceType surfaceType, BlendMode blendMode, CullMode cullMode)
	:shaderRef(shader), textures(textures), shininess(shininess), surfaceType(surfaceType),  blendMode(blendMode), cullMode(cullMode) {}

void Material::Apply()
{
	shaderRef->Use();
	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		textures[i].TextureSlot(GL_TEXTURE0 + static_cast<GLenum>(i));
		textures[i].Bind();
		shaderRef->SetInt("material." + textures[i].type, static_cast<int>(i));
	}
	shaderRef->SetFloat("material.shininess", shininess);


	switch (cullMode)
	{
	case CullMode::Back:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	case CullMode::Front:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	case CullMode::None:
		glDisable(GL_CULL_FACE);
		break;
	}

	if (surfaceType == SurfaceType::Transparent)
	{
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE); //NO DEPTH 

		switch (blendMode)
		{
		case BlendMode::Alpha:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BlendMode::PreMultiply:
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BlendMode::Additive:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case BlendMode::Multiply:
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
			break;
		}
	}
	else
	{
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}
}
