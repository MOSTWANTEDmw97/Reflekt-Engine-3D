#pragma once
#ifndef RENDERER_H
#define RENDERER_H


#include"Deferred_Rendering/DeferredLitBuffer.h"
#include"Deferred_Rendering/DeferredRenderer.h"
#include"Forward_Rendering/Transparent/TransparentRenderer.h"
#include"Forward_Rendering/Background/SkyboxRenderer.h"
#include"ScreenQuad.h"

class Renderer
{
	public:
		DeferredRenderer deferredRenderer;
		TransparentRenderer transparentRenderer;
		SkyboxRenderer skyboxRenderer;

		ScreenQuad screenQuad;

		Shader compositeShader;


		Renderer(int width, int height);

		void Render(Scene& scene);

		void OpaquePass(Scene& scene);
		void BackgroundPass(Scene& scene);
		void TransparentPass(Scene& scene);
		void CompositePass();
		
		void ChangeScreenResolution(int width, int height);

	private:
		int screenWidth, screenHeight;

};

#endif // !RENDERER_H
