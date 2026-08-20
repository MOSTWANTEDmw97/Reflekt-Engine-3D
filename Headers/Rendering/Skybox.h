#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include "GLBuffers/VAO.h"
#include "GLBuffers/VBO.h"
#include "Graphics/Cubemap.h"
#include "Graphics/Shader.h"

class Skybox
{
public:
    VAO vao;
    VBO vbo;
    Cubemap* cubemap;
    Shader* shader;

    Skybox(Cubemap* cubemap, Shader* shader);
    ~Skybox();

    void Draw(); // no extra params, same as other draw calls
};


#endif // !SKYBOX_H
