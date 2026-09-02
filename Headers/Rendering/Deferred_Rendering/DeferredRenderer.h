#pragma once
#ifndef DEFERRED_RENDERER_H
#define DEFERRED_RENDERER_H

#include<glad/glad.h>
#include"Graphics/Shader.h"
#include"Rendering/Deferred_Rendering/GBuffer.h"
#include"Rendering/Scene/Scene.h"
#include"DeferredLitBuffer.h"
#include"Rendering/ScreenQuad.h"

class DeferredRenderer
{
public:
    GBuffer gBuffer;
    DeferredLitBuffer litBuffer;
    Shader geometryPassShader;
    Shader lightingPassShader;

    ScreenQuad quad;

    int screenWidth, screenHeight;

    DeferredRenderer(int width, int height);

    void GeometryPass(Scene& scene);
    void LightingPass(Scene& scene);

    void RenderOpaque(Scene& scene);
    void ChangeScreenResolution(int width, int height);

private:
};

#endif // !DEFERRED_RENDERER_H
