#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad/glad.h>
#include<string>

class Texture
{
	public:
		GLuint ID;
		GLuint type;

		Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
		void Bind();
		void Unbind();
		void Delete();
};

#endif // !TEXTURE_H
