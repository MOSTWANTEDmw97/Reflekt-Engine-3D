#pragma once
#ifndef EBO_H
#define EBO_H

#include<glad/glad.h>

class EBO
{
	public:
		GLuint ID;
		EBO(const void* data, GLsizeiptr size);
		void Bind();
		void Unbind();
		void Delete();
};

#endif // !EBO_H

