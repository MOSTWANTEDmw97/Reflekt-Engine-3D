#include"Rendering/Deferred_Rendering/DeferredLitBuffer.h"

DeferredLitBuffer::DeferredLitBuffer(int width, int height)
{
    //fbo = FBO();
    fbo.Bind();

    // Color
    glGenTextures(1, &colorTex);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0,
        GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, colorTex, 0);

    //Depth
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, depthTex, 0);

    if (!IsComplete())
        std::cerr << "ERROR::DeferredLitBuffer:: Framebuffer is not complete!" << std::endl;
    else
        std::cout << "DeferredLitBuffer: Framebuffer complete" << std::endl;

    fbo.Unbind();
}

DeferredLitBuffer::~DeferredLitBuffer()
{
    glDeleteTextures(1, &colorTex);
    glDeleteTextures(1, &depthTex);
}

void DeferredLitBuffer::Bind()
{
    fbo.Bind();
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DeferredLitBuffer::Unbind()
{
    fbo.Unbind();
}

bool DeferredLitBuffer::IsComplete()
{
    return fbo.IsComplete();
}