#include"Rendering/Forward_Rendering/Background/SkyboxRenderer.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


SkyboxRenderer::SkyboxRenderer(int width, int height)
    :skyboxBuffer(width, height), 
    screenWidth(width), screenHeight(height)
{}

SkyboxRenderer::~SkyboxRenderer()
{}

void SkyboxRenderer::RenderSkybox(Skybox* _skybox)
{
    SetSkybox(_skybox);
    skyboxBuffer.Bind();

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    skybox->Draw();
    skyboxBuffer.Unbind();
}

void SkyboxRenderer::SetSkybox(Skybox* _skybox)
{
    skybox = _skybox;
}