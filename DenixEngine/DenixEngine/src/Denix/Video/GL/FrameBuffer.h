#pragma once

#include "Denix/Core.h"
#include "GL/glew.h"
#include "Denix/Video/Window/WindowSubsystem.h"

namespace Denix
{
	class FrameBuffer
	{
	public:
		FrameBuffer() : m_FboID(0) {}
		FrameBuffer(const int _width, const int _height) : m_FboID(0)
		{
			glGenFramebuffers(1, &m_FboID);
			if (!m_FboID) throw std::exception();
			glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);

			glGenTextures(1, &m_TexID);
			glBindTexture(GL_TEXTURE_2D, m_TexID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexID, 0);

			glGenRenderbuffers(1, &m_RboId);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RboId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RboId);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}


		void Bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_FboID); }
		static void Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
		GLuint GetTexureID() { return m_TexID; }

		GLuint m_FboID;
		GLuint m_TexID;
		GLuint m_RboId;
	};
}