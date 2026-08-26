#include"Rendering/Renderer.h"


Renderer::Renderer(int width, int height)
    : deferredRenderer(width, height),
    screenWidth(width),
    screenHeight(height),
    screenQuad() {}

void Renderer::Render(Scene& scene)
{
    deferredRenderer.RenderScene(scene);


    glDisable(GL_DEPTH_TEST);
    screenQuad.Draw();
    glEnable(GL_DEPTH_TEST);
}