#pragma once
#ifndef MESH_H
#define MESH_H

#include<glm.hpp>
#include<vector>
#include"VBO.h"
#include"VAO.h"
#include"EBO.h"
#include"Shader.h"
#include"Material.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoords;
};

class Mesh
{
	public:

		Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, Material mat, GLenum drawType = GL_STATIC_DRAW);
		void Draw(Shader& shader);
		void Delete();

		void SetMaterial(const Material& mat) { material = mat; }

		const std::vector<Vertex>& GetVertices() const { return vertices; }
		const std::vector<unsigned int>& GetIndices() const { return indices; }
		const Material& GetMaterial() const { return material; }

	private:
		void SetupMesh(GLenum drawType);
		Material material;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		VAO vao;
		VBO* vbo;
		EBO* ebo;
		GLsizei indexCount;
};

#endif // !MESH_H
