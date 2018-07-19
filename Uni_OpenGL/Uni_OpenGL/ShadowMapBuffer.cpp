#include "ShadowMapBuffer.h"
#include "ScreenManager.h"

#include <GLEW/glew.h>
#include "LogManager.h"

ShadowMapBuffer::ShadowMapBuffer()
{
}


ShadowMapBuffer::~ShadowMapBuffer()
{
	glDeleteFramebuffers(1, &_ID);
	glDeleteTextures(1, &_DepthMapID);
}

void ShadowMapBuffer::Create(int width, int height)
{
	_Width = width;
	_Height = height;
	glGenFramebuffers(1, &_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, _ID);

	glGenTextures(1, &_DepthMapID);
	glBindTexture(GL_TEXTURE_2D, _DepthMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, _Width, _Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _DepthMapID, 0);
	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LogManager::Instance()->LogError("Shadow FrameBuffer Broken...");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapBuffer::Bind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _ID);
	glViewport(0, 0, _Width, _Height);
}

void ShadowMapBuffer::Clear()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMapBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();
	glViewport(0, 0, screenSize.x, screenSize.y);
}
