#include "Rendering/Deferred_Rendering/DeferredRenderer.h"
#include<glad/glad.h>
#include<Camera.h>

DeferredRenderer::DeferredRenderer(int width, int height)
    : gBuffer(width, height),
    geometryPassShader("Default_Shaders/geometryPass.shader"),
    lightingPassShader("Default_Shaders/lightingPass.shader"),
    screenWidth(width),
    screenHeight(height),
    quad() {}

void DeferredRenderer::RenderScene(Scene& scene)
{

    BackgroundPass(scene);

    GeometryPass(scene);
    LightingPass(scene);

}

void DeferredRenderer::BackgroundPass(Scene& scene)
{
        if (scene.activeCamera && scene.activeCamera->skybox)
        {
            glDepthMask(GL_FALSE);          // don’t write depth
            glDepthFunc(GL_LEQUAL);         // pass if depth <= current
            scene.activeCamera->BindToShader(*scene.activeCamera->skybox->shader, glm::mat4(1.0f), (float)screenWidth / float(screenHeight));
            scene.activeCamera->skybox->Draw();
            glDepthMask(GL_TRUE);           // restore depth writes
            glDepthFunc(GL_LESS);           // restore default
        }
    
}

void DeferredRenderer::GeometryPass(Scene& scene)
{
    gBuffer.BindForGeometryPass();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //geometryPassShader.Use();
    scene.DrawGeometry((float)screenWidth, (float)screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::LightingPass(Scene& scene)
{
    gBuffer.BindForLightingPass();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lightingPassShader.Use();

    // Bind G‑Buffer textures
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

    // Upload lights
    scene.UploadLights(lightingPassShader);

    
    glDisable(GL_DEPTH_TEST);

    // Render full‑screen quad
    quad.Draw();
}

void DeferredRenderer::ChangeScreenResolution(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}
