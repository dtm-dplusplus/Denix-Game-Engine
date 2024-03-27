#pragma once

#include "Denix/Core.h"
#include <GL/glew.h>

namespace Denix
{
	class VertexArray
	{
	public:
		VertexArray()
		{
			glGenVertexArrays(1, &m_GL_ID);

			if (m_GL_ID) DE_LOG(LogGL, Trace, "Created VAO ID: {}", m_GL_ID)
			else DE_LOG(LogGL, Error, "Failed to create VAO")
		}

		~VertexArray()
		{
			if(m_GL_ID) glDeleteVertexArrays(1, &m_GL_ID);
		}

		void Bind() const { glBindVertexArray(m_GL_ID); }
		static void Unbind() { glBindVertexArray(0); }

		void AttribPtr(const GLsizei _size, const GLenum _type,
					   const GLboolean _normalized = false,
					   const void* _ptr = nullptr)
		{
			m_Stride += _size * sizeof(_type);
			glVertexAttribPointer(m_Index, _size, _type, _normalized, m_Stride, _ptr);
			glEnableVertexAttribArray(m_Index++); // Increment index for next attrib
		}

	private:
		GLuint m_GL_ID = 0;
		GLsizei m_Stride = 0;
		GLuint m_Index = 0;
	};
}