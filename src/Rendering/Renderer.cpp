#include"Rendering/Renderer.h"


Renderer::Renderer(int width, int height)
    : deferredRenderer(width, height),
    transparentRenderer(width, height),
    skyboxRenderer(width, height),
    screenWidth(width),
    screenHeight(height),
    screenQuad(),
    compositeShader("Default_Shaders/composite.shader") { }

void Renderer::Render(Scene& scene)
{
    OpaquePass(scene);
    BackgroundPass(scene);
    TransparentPass(scene);

    //Shader defaultS("Default_Shaders/olddefault_lit.shader");
    CompositePass();
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    screenQuad.Draw();
    glEnable(GL_DEPTH_TEST);
}

void Renderer::OpaquePass(Scene& scene)
{
    deferredRenderer.RenderOpaque(scene);
}

void Renderer::BackgroundPass(Scene& scene)
{
    Camera* cam = scene.activeCamera;
    if (cam && cam->skybox)
    {
        cam->BindToShader(*cam->skybox->shader, cam->transform.GetModelMatrix(), float(screenWidth) / float(screenHeight));
        skyboxRenderer.RenderSkybox(cam->skybox);
    }

}

void Renderer::TransparentPass(Scene& scene)
{

    transparentRenderer.RenderTransparent(scene);
}

void Renderer::CompositePass()
{

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // backbuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    compositeShader.Use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, deferredRenderer.litBuffer.colorTex);
    compositeShader.SetInt("opaqueTex", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, skyboxRenderer.skyboxBuffer.colorTex);
    compositeShader.SetInt("backgroundTex", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, transparentRenderer.transparentBuffer.colorTex);
    compositeShader.SetInt("transparentTex", 2);

    //glDisable(GL_DEPTH_TEST);
    //screenQuad.Draw();
    //glEnable(GL_DEPTH_TEST);
}