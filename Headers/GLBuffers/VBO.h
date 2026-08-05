#pragma once
#ifndef VBO_H
#define VBO_H

#include<glad/glad.h>
class VBO
{
	public:
		GLuint ID;
		VBO(const void* data, GLsizeiptr size, GLenum drawType);
		void Bind();
		void Unbind();
		void Delete();
};

#endif // !VBO_H
