#include"Graphics/ShaderManager.h"

std::unordered_map<std::string, Shader> ShaderManager::shaders;

void ShaderManager::Register(const Shader& shader, const std::string& name)
{
    shaders.emplace(name, shader);
}

Shader& ShaderManager::Get(const std::string& name)
{
    return shaders.at(name); // throws std::out_of_range if not found
}

bool ShaderManager::Exists(const std::string& name)
{
    return shaders.find(name) != shaders.end();
}

void ShaderManager::Clear()
{
    shaders.clear();
}