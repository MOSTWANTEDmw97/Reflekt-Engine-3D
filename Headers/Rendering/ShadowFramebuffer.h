#pragma once
#ifndef SHADOW_FRAMEBUFFER_H
#define SHADOW_FRAMEBUFFER_H

#include "GLBuffers/FBO.h"
#include <glad/glad.h>
#include <iostream>

class ShadowFramebuffer : public FBO
{
public:
    GLuint depthTex;

    ShadowFramebuffer(int width, int height);
    ~ShadowFramebuffer();

    // Accessor
    GLuint GetDepthTex() const { return depthTex; }
};

#endif