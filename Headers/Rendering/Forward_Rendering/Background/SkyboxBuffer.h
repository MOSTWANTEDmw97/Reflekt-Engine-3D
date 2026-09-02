#pragma once
#ifndef SKYBOXBUFFER_H
#define SKYBOXBUFFER_H

#include"GLBuffers/FBO.h"
#include<glad/glad.h>

class SkyboxBuffer
{
	public:
		FBO fbo;
		GLuint colorTex;
		GLuint depthTex;


		SkyboxBuffer(int width, int height);
		~SkyboxBuffer();

		void Bind();
		void Unbind();
		bool IsComplete();

	private:
		int screenWidth, screenHeight;
};

#endif // !SKYBOXBUFFER_H
