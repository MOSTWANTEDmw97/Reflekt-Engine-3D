#include"Graphics/Skybox.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// Cube vertices (positions only)
static GLfloat skyboxVertices[] = {
    // positions          
 -1.0f,  1.0f, -1.0f,
 -1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,
 -1.0f,  1.0f, -1.0f,

 -1.0f, -1.0f,  1.0f,
 -1.0f, -1.0f, -1.0f,
 -1.0f,  1.0f, -1.0f,
 -1.0f,  1.0f, -1.0f,
 -1.0f,  1.0f,  1.0f,
 -1.0f, -1.0f,  1.0f,

  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,

 -1.0f, -1.0f,  1.0f,
 -1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f, -1.0f,  1.0f,
 -1.0f, -1.0f,  1.0f,

 -1.0f,  1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
 -1.0f,  1.0f,  1.0f,
 -1.0f,  1.0f, -1.0f,

 -1.0f, -1.0f, -1.0f,
 -1.0f, -1.0f,  1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
 -1.0f, -1.0f,  1.0f,
  1.0f, -1.0f,  1.0f
};

Skybox::Skybox(Cubemap* cubemap, Shader* shader)
    : cubemap(cubemap), shader(shader),
    vao(), vbo(skyboxVertices, sizeof(skyboxVertices), GL_STATIC_DRAW)
{
    vao.Bind();
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    vao.Unbind();
    vbo.Unbind();
}

Skybox::~Skybox()
{
    vao.Delete();
    vbo.Delete();
}

void Skybox::SetCubemap(Cubemap* _cubemap)
{
    cubemap = _cubemap;
}
void Skybox::SetShader(Shader* _shader)
{
    shader = _shader;
}

void Skybox::Draw()
{
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glDepthMask(GL_TRUE);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->Use();
    cubemap->Bind(0);

    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    vao.Unbind();

    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}

