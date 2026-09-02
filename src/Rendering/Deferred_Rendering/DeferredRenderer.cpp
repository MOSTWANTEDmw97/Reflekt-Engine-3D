#include "Rendering/Deferred_Rendering/DeferredRenderer.h"
#include<glad/glad.h>
#include<Camera.h>

//Deferred Renderer only is supposed to render Opaque objects 

DeferredRenderer::DeferredRenderer(int width, int height)
    : gBuffer(width, height),
    litBuffer(width, height),
    geometryPassShader("Default_Shaders/geometryPass.shader"),
    lightingPassShader("Default_Shaders/lightingPass.shader"),
    screenWidth(width),
    screenHeight(height), 
    quad() {}

void DeferredRenderer::RenderOpaque(Scene& scene)
{
    GeometryPass(scene);
    LightingPass(scene);
}


void DeferredRenderer::GeometryPass(Scene& scene)
{
    gBuffer.BindForGeometryPass();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_DEPTH_BUFFER_BIT);
    scene.DrawOpaqueGeometry((float)screenWidth, (float)screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::LightingPass(Scene& scene)
{
    //gBuffer.BindForLightingPass();
    litBuffer.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    lightingPassShader.Use();

    //Bind G‑Buffer tex
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
    lightingPassShader.SetInt("gPosition", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
    lightingPassShader.SetInt("gNormal", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gAlbedo);
    lightingPassShader.SetInt("gAlbedo", 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gSpecular);
    lightingPassShader.SetInt("gSpecular", 3);

    scene.UploadLights(lightingPassShader);

    glDisable(GL_DEPTH_TEST);

    quad.Draw();
    litBuffer.Unbind();
    glEnable(GL_DEPTH_TEST);
}

void DeferredRenderer::ChangeScreenResolution(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}
