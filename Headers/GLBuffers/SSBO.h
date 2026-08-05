#pragma once
#ifndef SSBO_H
#define SSBO_H

#include<glad/glad.h>

class SSBO
{
	public:
		GLuint ID;
		SSBO(GLuint bindingPoint);
		void Bind();
		void Unbind();
		void Delete();

		void Upload(const void* data, GLsizeiptr size, GLenum drawType = GL_DYNAMIC_DRAW);

	private:
		GLuint bindingPoint;
};

#endif // !SSBO_H
