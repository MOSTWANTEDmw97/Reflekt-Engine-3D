#include "Rendering/RenderSceneFramebuffer.h"

RenderSceneFramebuffer::RenderSceneFramebuffer(int width, int height)
{
    Bind();

    // Color attachment
    glGenTextures(1, &colorTex);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, colorTex, 0);

    // Depth + stencil attachment
    glGenTextures(1, &depthStencilTex);
    glBindTexture(GL_TEXTURE_2D, depthStencilTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
        GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_TEXTURE_2D, depthStencilTex, 0);

    if (!IsComplete())
    {
        std::cerr << "ERROR::RENDER_SCENE_FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

    Unbind();
}

RenderSceneFramebuffer::~RenderSceneFramebuffer()
{
    glDeleteTextures(1, &colorTex);
    glDeleteTextures(1, &depthStencilTex);
}