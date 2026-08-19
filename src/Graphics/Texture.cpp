#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#include<iostream>
#include<algorithm>
#include"Graphics/Texture.h"


// External texture 
Texture::Texture(const char* imagePath, const std::string& typeName, GLenum format, GLenum texType, GLenum slot, GLenum pixelType, GLenum filterMode, GLenum wrapMode)
	:ID(0), 
	Type(texType),Slot(slot), PixelType(pixelType)
{
	std::string typeNameLower = typeName;
	std::transform(typeNameLower.begin(), typeNameLower.end(), typeNameLower.begin(),
		[](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	this->type = typeNameLower;

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Failed to load texture: "<< imagePath << std::endl;
		return;
	}


	GLenum texFormat = format;
	if (nrChannels == 1)      texFormat = GL_RED;
	else if (nrChannels == 3) texFormat = GL_RGB;
	else if (nrChannels == 4) texFormat = GL_RGBA;
		

	//std::cout << "Image: " << imagePath << format << " Tex: "<<texFormat <<" nr: " << nrChannels << std::endl;
 
	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexImage2D(texType, 0, format, width, height, 0, texFormat, pixelType, data);
	glGenerateMipmap(texType);

	stbi_image_free(data);
	glBindTexture(texType, 0);
}


//Embedded Assimp texture 
Texture::Texture(const aiTexture* aiTex,
	const std::string& typeName,
	GLenum texType,
	GLenum slot,
	GLenum pixelType,
	GLenum filterMode,
	GLenum wrapMode)
	: ID(0), Type(texType), Slot(slot), PixelType(pixelType)
{
	std::string typeNameLower = typeName;
	std::transform(typeNameLower.begin(), typeNameLower.end(), typeNameLower.begin(),
		[](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	this->type = typeNameLower;

	// Assimp embedded textures can be compressed (jpg/png) or raw RGBA
	int width, height, nrChannels;
	unsigned char* data = nullptr;

	if (aiTex->mHeight == 0)
	{
		// Compressed texture (e.g. jpg/png in memory)
		data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(aiTex->pcData),
			aiTex->mWidth, &width, &height, &nrChannels, 0);
	}
	else
	{
		// Raw RGBA texture
		width = aiTex->mWidth;
		height = aiTex->mHeight;
		nrChannels = 4;
		data = reinterpret_cast<unsigned char*>(aiTex->pcData);
	}

	if (!data)
	{
		std::cerr << "Failed to load embedded texture" << std::endl;
		return;
	}

	GLenum texFormat = GL_RGBA;
	if (nrChannels == 1) texFormat = GL_RED;
	else if (nrChannels == 3) texFormat = GL_RGB;
	else if (nrChannels == 4) texFormat = GL_RGBA;

	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, wrapMode);

	glTexImage2D(texType, 0, texFormat, width, height, 0, texFormat, pixelType, data);
	glGenerateMipmap(texType);

	if (aiTex->mHeight == 0)
	{
		stbi_image_free(data); // only free if stbi allocated
	}

	glBindTexture(texType, 0);
}

void Texture::TextureType(GLenum texType)
{
	Type = texType;
}
void Texture::TextureSlot(GLenum slot)
{
	Slot = slot;
	glActiveTexture(slot);
	glBindTexture(Type, ID);
}

void Texture::TextureFilterMode(GLenum minFilter, GLenum magFilter)
{
	glBindTexture(Type, ID);
	glTexParameteri(Type, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(Type, GL_TEXTURE_MAG_FILTER, magFilter);
	glBindTexture(Type, 0);
}
void Texture::TextureWrapMode(GLenum wrapS, GLenum wrapT)
{
	glBindTexture(Type, ID);
	glTexParameteri(Type, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(Type, GL_TEXTURE_WRAP_T, wrapT);
	glBindTexture(Type, 0);
}

void Texture::Bind()
{
	glBindTexture(Type, ID);
}
void Texture::Unbind()
{
	glBindTexture(Type, 0);
}
void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}
