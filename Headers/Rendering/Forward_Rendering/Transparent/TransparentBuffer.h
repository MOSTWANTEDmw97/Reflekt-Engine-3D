#pragma once
#ifndef TRANSPARENTBUFFER_H
#define TRANSPARENTBUFFER_H

#include<glad/glad.h>
#include"GLBuffers/FBO.h"

class TransparentBuffer
{
public:
    FBO fbo;
    GLuint colorTex;
    GLuint depthTex;



    TransparentBuffer(int width, int height);
    ~TransparentBuffer();
//
    void Bind();
    void Unbind();
    bool IsComplete();

    private:
        int screenWidth, screenHeight;
};


#endif // !TRANSPARENTBUFFER_H
