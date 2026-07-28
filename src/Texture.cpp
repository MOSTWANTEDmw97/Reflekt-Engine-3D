#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#include<iostream>
#include"Texture.h"

Texture::Texture(const char* imagePath, GLenum format, GLenum texType, GLenum slot, GLenum pixelType, GLenum filterMode, GLenum wrapMode)
	:ID(0),
	Type(texType),Slot(slot), PixelType(pixelType)
{
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Failed to load texture" << std::endl;
		return;
	}

	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexImage2D(texType, 0, format, width, height, 0, format, pixelType, data);
	glGenerateMipmap(texType);

	stbi_image_free(data);
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
