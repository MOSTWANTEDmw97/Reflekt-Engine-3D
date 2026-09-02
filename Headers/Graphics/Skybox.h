#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H
#pragma once

#include "GLBuffers/VAO.h"
#include "GLBuffers/VBO.h"
#include "Graphics/Cubemap.h"
#include "Graphics/Shader.h"

class Skybox
{
public:
    Cubemap* cubemap;
    Shader* shader;

    Skybox(Cubemap* cubemap, Shader* shader);
    ~Skybox();

    void SetCubemap(Cubemap* _cubemap);
    void SetShader(Shader* _shader);
    void Draw();
private:
    int screenWidth, screenHeight;
    VAO vao;
    VBO vbo;
};


#endif // !SKYBOX_H

