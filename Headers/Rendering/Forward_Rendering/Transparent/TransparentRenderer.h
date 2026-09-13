#pragma once
#ifndef TRANSPARENTRENDERER_H
#define TRANSPARENTRENDERER_H

#include<glad/glad.h>
#include"Graphics/Shader.h"
#include"Rendering/Scene/Scene.h"
#include"Rendering/ScreenQuad.h"
#include"Rendering/Forward_Rendering/Transparent/TransparentBuffer.h"
#include"Rendering/Deferred_Rendering/DeferredRenderer.h"

class TransparentRenderer
{
	public:
		Shader forwardShader;
		TransparentBuffer transparentBuffer;
		int screenWidth, screenHeight;

		ScreenQuad quad;
		TransparentRenderer(int width, int height);

		void RenderTransparent(Scene& scene);


		void ChangeScreenResolution(int width, int height);

};

#endif


