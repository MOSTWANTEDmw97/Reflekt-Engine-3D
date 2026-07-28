#pragma once
#ifndef SHADER_H
#define SHADER_H

#include<iostream>
#include<string>
#include<glad/glad.h>
#include<glm.hpp>

class Shader
{
	public:
		GLuint ID;

		// Constructor
		Shader(const char* vertexPath, const char* fragmentPath);

		// Activate
		void Use();
		void Delete();

		void SetMat4(const std::string& name, const glm::mat4& mat) const;

		// utils
		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetVec3(const std::string& name, glm::vec3 value) const;


	private:
		std::string LoadShaderSource(const char* shaderPath);
		void CheckCompileErrors(GLuint shader, std::string type);
};

#endif

