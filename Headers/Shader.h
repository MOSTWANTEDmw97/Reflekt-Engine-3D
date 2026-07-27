#pragma once
#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<glad/glad.h>

class Shader
{
	public:
		GLuint ID;

		// Constructor
		Shader(const char* vertexPath, const char* fragmentPath);

		// Activate
		void Use();
		void Delete();

		// utils
		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;


	private:
		std::string LoadShaderSource(const char* shaderPath);
		void CheckCompileErrors(GLuint shader, std::string type);
};

#endif

