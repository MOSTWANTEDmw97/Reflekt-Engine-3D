#pragma once
#ifndef CUBEMAP_H
#define CUBEMAP_H

#include<glad/glad.h>
#include<string>
#include<vector>

class Cubemap
{
    public:
	    GLuint textureID;

        Cubemap(const std::vector<std::string>& faces);
        ~Cubemap();

        void Bind(GLuint unit = 0) const;
};

#endif // !CUBEMAP_H
