#pragma once
#pragma once
#ifndef GBUFFER_H
#define GBUFFER_H

#include <glad/glad.h>
#include <iostream>
#include "GLBuffers/FBO.h"

class GBuffer : public FBO
{
public:
    GLuint gPosition;
    GLuint gNormal;
    GLuint gAlbedo;
    GLuint gSpecular;
    GLuint rboDepth;

    GBuffer(int width, int height);
    ~GBuffer();

    void BindForGeometryPass();
    void BindForLightingPass();
};

#endif // !GBUFFER_H
