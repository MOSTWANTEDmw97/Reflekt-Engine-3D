#include"Rendering/ScreenQuad.h"

ScreenQuad::ScreenQuad()
{
    // positions + texcoords
    float quadVertices[] = {
        // pos      // tex
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    vertexCount = 6;

    vao.Bind();
    vbo = new VBO(quadVertices, sizeof(quadVertices), GL_STATIC_DRAW);

    // layout 0: position (vec2)
    vao.LinkAttrib(*vbo, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
    // layout 1: texcoords (vec2)
    vao.LinkAttrib(*vbo, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    vao.Unbind();
    vbo->Unbind();
}

ScreenQuad::~ScreenQuad()
{
    vbo->Delete();
    delete vbo;
    vao.Delete();
}

void ScreenQuad::Draw()
{
    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    vao.Unbind();
}