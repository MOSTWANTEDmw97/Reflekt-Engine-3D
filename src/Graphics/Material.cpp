#include"Graphics/Material.h"

Material::Material(Shader* shader, const std::vector<Texture>& textures, float shininess, BlendMode blendmode)
	:shaderRef(shader), textures(textures), shininess(shininess), blendMode(blendmode) {}

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
}
