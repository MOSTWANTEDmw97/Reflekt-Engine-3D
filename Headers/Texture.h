#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad/glad.h>
#include<string>

class Texture
{
	public:
		GLuint ID;
		GLuint Type;
		GLuint Slot;
		GLuint PixelType;

		Texture(const char* imagePath,
			GLenum format,
			GLenum texType = GL_TEXTURE_2D,
			GLenum slot = GL_TEXTURE0,
			GLenum pixelType = GL_UNSIGNED_BYTE,
			GLenum filterMode = GL_LINEAR,
			GLenum wrapMode = GL_REPEAT);
		void Bind();
		void Unbind();
		void Delete();

		//Helpers and setters
		void TextureType(GLenum texType);
		void TextureSlot(GLenum slot);
		void TexturePixelType(GLenum pixelType);
		void TextureFilterMode(GLenum minFilter, GLenum magFilter);
		void TextureWrapMode(GLenum wrapS, GLenum wrapT);

};

#endif // !TEXTURE_H
