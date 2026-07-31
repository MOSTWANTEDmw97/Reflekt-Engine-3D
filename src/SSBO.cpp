#include"SSBO.h"

SSBO::SSBO(GLuint bindingPoint)
	: bindingPoint(bindingPoint)
{
	glGenBuffers(1, &ID);
}

void SSBO::Bind()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, ID);
}
void SSBO::Unbind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
void SSBO::Delete()
{
	glDeleteBuffers(1, &ID);
}

void SSBO::Upload(const void* data, GLsizeiptr size, GLenum drawType)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, drawType);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, ID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}