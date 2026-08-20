#include"Mesh.h"
#include<iostream>

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, Material mat,GLenum drawType)
	:vertices(std::move(verts)), indices(std::move(inds)), material(std::move(mat)),
	indexCount(static_cast<GLsizei>(indices.size()))
{

	SetupMesh(drawType);
}

void Mesh::SetupMesh(GLenum drawType)
{
	vao.Bind();
	vbo = new VBO(vertices.data(), vertices.size() * sizeof(Vertex), drawType);
	ebo = new EBO(indices.data(), indices.size() * sizeof(unsigned int));

	//Link attribs
	vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	vao.LinkAttrib(*vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	vao.LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));
	vao.LinkAttrib(*vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	vao.Unbind();
}

void Mesh::Draw()
{
	material.Apply();

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