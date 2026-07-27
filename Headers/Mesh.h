#pragma once
#ifndef MESH_H
#define MESH_H

#include"VBO.h"
#include"VAO.h"
#include"EBO.h"

class Mesh
{
	public:
		VAO vao;
		VBO* vbo;
		EBO* ebo;
		GLsizei indexCount;

		Mesh(const GLfloat* vertices, GLsizeiptr vertSize, const GLuint* indices, GLsizeiptr indicesSize, GLenum drawType);
		void Draw();
		void Delete();

};

#endif // !MESH_H
