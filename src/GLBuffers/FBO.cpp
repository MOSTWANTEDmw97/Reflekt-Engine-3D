#include"GLBuffers/FBO.h"

FBO::FBO()
{

	glGenFramebuffers(1, &ID);
}

FBO::~FBO()
{
	glDeleteFramebuffers(1, &ID);
}

void FBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}
void FBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FBO::IsComplete()
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}