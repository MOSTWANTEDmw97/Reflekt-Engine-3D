#pragma once
#ifndef SKYBOXRENDERER_H
#define SKYBOXRENDERER_H

#include"Graphics/Skybox.h"
#include"Rendering/Forward_Rendering/Background/SkyboxBuffer.h"

class SkyboxRenderer
{
    public:
        Skybox* skybox;
        SkyboxBuffer skyboxBuffer;

        SkyboxRenderer(int width, int height);
        ~SkyboxRenderer();

        void SetSkybox(Skybox* skybox); 
        void RenderSkybox(Skybox* skybox);
    private:
        int screenWidth, screenHeight;
};


#endif // !SKYBOX_H
