#include"Rendering/Forward_Rendering/Transparent/TransparentRenderer.h"

TransparentRenderer::TransparentRenderer(int width, int height)
    : screenWidth(width), screenHeight(height),
    forwardShader("Default_Shaders/transparent_lit.shader"),
    transparentBuffer(width, height), quad() {}

void TransparentRenderer::RenderTransparent(Scene& scene)
{
    transparentBuffer.Bind();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    scene.UploadLights(forwardShader);
    scene.DrawTransparentGeometry(float(screenWidth), float(screenHeight));
    
    transparentBuffer.Unbind();
}