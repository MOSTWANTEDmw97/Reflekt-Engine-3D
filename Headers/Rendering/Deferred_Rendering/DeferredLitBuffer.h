#pragma once
#ifndef DEFERREDLITBUFFER_H
#define DEFERREDLITBUFFER_H

#include <glad/glad.h>
#include <iostream>
#include"GLBuffers/FBO.h"

class DeferredLitBuffer
{
    public:
        FBO fbo;

        GLuint colorTex;
        GLuint depthTex;

        DeferredLitBuffer(int width, int height);
        ~DeferredLitBuffer();


        void Bind();
        void Unbind();
        bool IsComplete();

};



#endif // !DEFERREDLITBUFFER_H

