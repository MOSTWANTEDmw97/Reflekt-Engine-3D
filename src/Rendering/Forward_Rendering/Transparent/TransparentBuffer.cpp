#include"Rendering/Forward_Rendering/Transparent/TransparentBuffer.h"

TransparentBuffer::TransparentBuffer(int width, int height)
    : screenWidth(width), screenHeight(height), fbo()
{
    fbo.Bind();

    // Color attachment (RGBA with alpha for blending)
    glGenTextures(1, &colorTex);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0,
        GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, colorTex, 0);

    // Depth attachment
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
        width, height, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, depthTex, 0);

    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    if (!IsComplete())
        std::cerr << "ERROR::TransparentBuffer:: Framebuffer not complete!" << std::endl;
    else
        std::cout << "TransparentBuffer: Framebuffer complete" << std::endl;

    fbo.Unbind();
}

TransparentBuffer::~TransparentBuffer()
{
    glDeleteTextures(1, &colorTex);
    glDeleteTextures(1, &depthTex);
}

void TransparentBuffer::Bind()
{
    fbo.Bind();
}

void TransparentBuffer::Unbind()
{
    fbo.Unbind();
}

bool TransparentBuffer::IsComplete()
{
    return fbo.IsComplete();
}
