#pragma once
#ifndef RENDERER_H
#define RENDERER_H


#include"Deferred_Rendering/DeferredLitBuffer.h"
#include"Deferred_Rendering/DeferredRenderer.h"
#include"ScreenQuad.h"

class Renderer
{
	public:
		DeferredRenderer deferredRenderer;
		ScreenQuad screenQuad;

		int screenWidth;
		int screenHeight;

		Renderer(int width, int height);

		void Render(Scene& scene);
		void ChangeScreenResolution(int width, int height);

};

#endif // !RENDERER_H
