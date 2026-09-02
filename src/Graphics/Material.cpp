#include"Graphics/Material.h"


Material::Material(Shader* shader)
	:shaderRef(shader)	{}

void Material::Apply(Shader* overrideShader)
{
	Shader* activeShader = overrideShader ? overrideShader : shaderRef;

	if (!activeShader) { std::cerr << "Material::Apply: no shader\n"; return; }
	activeShader->Use();
	
	//Set uniforms for shader
	for (auto& [key, value] : floatProperties) activeShader->SetFloat(key, value);
	for (auto& [key, value] : intProperties) activeShader->SetInt(key, value);
	for (auto& [key, value] : boolProperties) activeShader->SetBool(key, value);
	for (auto& [key, value] : vec3Properties) activeShader->SetVec3(key, value);
	for (auto& [key, value] : mat4Properties) activeShader->SetMat4(key, value);
	//DO SO FOR VEC2, VEC4
	for (auto& [key, value] : textureProperties) //Texture2D
	{
		int slot = value.slot;
		value.tex->TextureSlot(GL_TEXTURE0 + static_cast<GLenum>(slot));
		value.tex->Bind();
		activeShader->SetInt(key, slot);
	}
	for (auto& [key, value] : cubemapTextureProperties) //Cubemap
	{
		int slot = value.slot;
		value.tex->TextureSlot(GL_TEXTURE0 + static_cast<GLenum>(slot));
		value.tex->Bind();
		activeShader->SetInt(key, slot);
	}

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
		glDepthMask(GL_TRUE); //NO DEPTH
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
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}
}


//Shader property Setters
void Material::SetFloat(const std::string& name, float v) { floatProperties[name] = v; }
void Material::SetInt(const std::string& name, int v) { intProperties[name] = v; }
void Material::SetBool(const std::string& name, bool v) { boolProperties[name] = v; }
void Material::SetVec2(const std::string& name, const glm::vec2& v) { vec2Properties[name] = v; }
void Material::SetVec3(const std::string& name, const glm::vec3& v) { vec3Properties[name] = v; }
void Material::SetVec4(const std::string& name, const glm::vec4& v) { vec4Properties[name] = v; }
void Material::SetMat4(const std::string& name, const glm::mat4& v) { mat4Properties[name] = v; }
void Material::SetTexture(const std::string& name, Texture* tex, int slot) { textureProperties[name] = { tex, slot }; }
void Material::SetCubeMapTexture(const std::string& name, Texture* tex, int slot) { cubemapTextureProperties[name] = { tex, slot }; }
