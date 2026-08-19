#include<iostream>
#include<fstream>
#include<sstream>
#include<filesystem>
#include<string>
#include<gtc/type_ptr.hpp>
#include"Graphics/Shader.h"


static std::string ExtractShaderStage(const std::string& source, const std::string& tag)
{
	size_t start = source.find(tag);
	if (start == std::string::npos) return "";

	size_t end = source.find("#SHADER", start + tag.size());
	return source.substr(start + tag.size(), end - (start + tag.size()));
}
//Single shader file
Shader::Shader(const char* filePath)
{
	std::string shaderCode = LoadShaderSource(filePath);

	std::string vertexCode = ExtractShaderStage(shaderCode, "#SHADER VERTEX");
	std::string fragmentCode = ExtractShaderStage(shaderCode, "#SHADER FRAGMENT");
	std::string geometryCode = ExtractShaderStage(shaderCode, "#SHADER GEOMETRY");
	std::string tessControlCode = ExtractShaderStage(shaderCode, "#SHADER TESS_CONTROL"); //Tesselation Control
	std::string tessEvaluationCode = ExtractShaderStage(shaderCode, "#SHADER TESS_EVALUATION"); //Tesselation Evaluation
	std::string computeCode = ExtractShaderStage(shaderCode, "#SHADER COMPUTE"); //Compute shader

	ID = glCreateProgram();

	auto CompileShaderStage = [&](const std::string& src, GLenum type, const std::string& name)
		{
			if (src.empty())
			{
				//std::cout << "No " << name << " stage found.\n";
				return GLuint(0);
			}
			GLuint shader = glCreateShader(type);
			const char* code = src.c_str();
			glShaderSource(shader, 1, &code, nullptr);
			glCompileShader(shader);
			CheckCompileErrors(shader, name);
			glAttachShader(ID, shader);
			return shader;
		};

	GLuint vertex = CompileShaderStage(vertexCode, GL_VERTEX_SHADER, "VERTEX");
	GLuint fragment = CompileShaderStage(fragmentCode, GL_FRAGMENT_SHADER, "FRAGMENT");
	GLuint geometry = CompileShaderStage(geometryCode, GL_GEOMETRY_SHADER, "GEOMETRY");
	GLuint tessControl = CompileShaderStage(tessControlCode, GL_TESS_CONTROL_SHADER, "TESS_CONTROL");
	GLuint tessEvaluation = CompileShaderStage(tessEvaluationCode, GL_TESS_EVALUATION_SHADER, "TESS_EVALUATION");
	GLuint compute = CompileShaderStage(computeCode, GL_COMPUTE_SHADER, "COMPUTE");

	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");

	//Delete if not 0
	if (vertex)   glDeleteShader(vertex);
	if (fragment) glDeleteShader(fragment);
	if (geometry) glDeleteShader(geometry);
	if (tessControl) glDeleteShader(tessControl);
	if (tessEvaluation) glDeleteShader(tessEvaluation);
	if (compute)  glDeleteShader(compute);

}

//Seperate shader files
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode = LoadShaderSource(vertexPath);
	std::string fragmentCode = LoadShaderSource(fragmentPath);
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");

	// Shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");
	// Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use()
{
	glUseProgram(ID);
}
void Shader::Delete()
{
	glDeleteProgram(ID);
}

// Uniforms
void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

// Private, Shader load and compile error report
std::string Shader::LoadShaderSource(const char* shaderPath)
{
	std::filesystem::path exePath = std::filesystem::current_path();
	std::filesystem::path fullPath = exePath / shaderPath;

	std::ifstream shaderFile(fullPath);
	if (!shaderFile.is_open())
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_FOUND: " << fullPath << std::endl;
		return "";
	}

	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	return shaderStream.str();
}

void Shader::CheckCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];

	if (shader == 0)
	{
		std::cout << "ERROR::SHADER::SHADER_NOT_CREATED" << std::endl;
		return;	
	}
	else if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog  << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
		}
	}
}

