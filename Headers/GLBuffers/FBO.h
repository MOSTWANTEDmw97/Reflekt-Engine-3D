#pragma once
#ifndef FBO_H
#define FBO_H

#include<glad/glad.h>
#include<iostream>

class FBO
{
	public:
		GLuint ID;
		GLuint colorTex;
		GLuint depthStencilTex;

		FBO(int width, int height);
		~FBO();

		void Bind();
		void Unbind();
		bool IsComplete();
};


#endif // !FBO_H
