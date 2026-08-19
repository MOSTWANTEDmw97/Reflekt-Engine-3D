#pragma once
#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <unordered_map>
#include <string>
#include "Shader.h"

class ShaderManager
{
    public:
        static void Register(const Shader& shader, const std::string& name);

        static Shader& Get(const std::string& name);

        static bool Exists(const std::string& name);
        static void Clear();

    private:
        static std::unordered_map<std::string, Shader> shaders;
};


#endif // !SHADERMANAGER_H
