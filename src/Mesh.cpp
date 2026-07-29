#include"Mesh.h"
#include<iostream>

Mesh::Mesh(const GLfloat* vertices, GLsizeiptr vertSize, const GLuint* indices, GLsizeiptr indicesSize, GLenum drawType)
{
	indexCount = static_cast<GLsizei>(indicesSize / sizeof(GLuint));

	vao.Bind();
	vbo = new VBO(vertices, vertSize, drawType);
	ebo = new EBO(indices, indicesSize);

	vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);              // position xyz
	vao.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float))); // normal xyz
	vao.LinkAttrib(*vbo, 2, 3, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float))); // color rgb
	vao.LinkAttrib(*vbo, 3, 2, GL_FLOAT, 11 * sizeof(float), (void*)(9 * sizeof(float))); // texcoords uv


	vao.Unbind();
}

void Mesh::Draw()
{
	vao.Bind();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	//std::cout << "Index: " <<indexCount<< std::endl;
	vao.Unbind();
}

void Mesh::Delete()
{
	vao.Delete();
	vbo->Delete();
	ebo->Delete();
	delete vbo;
	delete ebo;
}