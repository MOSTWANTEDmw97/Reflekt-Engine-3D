#pragma once
#ifndef SCREENQUAD_H
#define SCREENQUAD_H

#include"GLBuffers/VAO.h"
#include"GLBuffers/VBO.h"

class ScreenQuad
{
	public:

        VAO vao;
        VBO* vbo;
        GLuint vertexCount;

        ScreenQuad();
        ~ScreenQuad();

        void Draw();

};

#endif // !SCREENQUAD_H
