#include"Rendering/Skybox.h"

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

void Skybox::Draw()
{
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    shader->Use();
    cubemap->Bind(0);

    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    vao.Unbind();

   glDepthFunc(GL_LESS);
   glDisable(GL_CULL_FACE);
   glDepthMask(GL_TRUE);
}
