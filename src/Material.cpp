#include"Material.h"

Material::Material(const std::vector<Texture>& textures, float shininess)
	: textures(textures), shininess(shininess) {}

void Material::Apply(Shader& shader)
{
	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		textures[i].TextureSlot(GL_TEXTURE0 + static_cast<GLenum>(i));
		textures[i].Bind();
		shader.SetInt("material." + textures[i].type, static_cast<int>(i));
	}
	shader.SetFloat("material.shininess", shininess);
}
