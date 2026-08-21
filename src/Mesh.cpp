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

void Mesh::SetupInstanceBuffer(const std::vector<glm::mat4>& transforms)
{
	vao.Bind();
	instanceVBO = new VBO(transforms.data(), transforms.size() * sizeof(glm::mat4), GL_STATIC_DRAW);

	GLsizei vec4Size = sizeof(glm::vec4);
	for (int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(4 + i);
		glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * vec4Size));
		glVertexAttribDivisor(4 + i, 1);
	}
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

void Mesh::DrawInstanced(GLsizei instanceCount)
{
	material.Apply();
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, instanceCount);
	vao.Unbind();
}

void Mesh::Delete()
{
	vao.Delete();
	vbo->Delete();
	ebo->Delete();
	if (instanceVBO) instanceVBO->Delete();
	delete vbo;
	delete ebo;
	if (instanceVBO) delete instanceVBO;
}