#pragma once
#ifndef RENDERSCENEFRAMEBUFFER_H
#define RENDERSCENEFRAMEBUFFER_H

#include "GLBuffers/FBO.h"
#include <glad/glad.h>
#include <iostream>

class RenderSceneFramebuffer : public FBO
{
    public:
        GLuint colorTex;
        GLuint depthStencilTex;

        RenderSceneFramebuffer(int width, int height);
        ~RenderSceneFramebuffer();

        // Accessors
        GLuint GetColorTex() const { return colorTex; }
        GLuint GetDepthStencilTex() const { return depthStencilTex; }
};

#endif // !RENDERSCENEFRAMEBUFFER_H
