#pragma once

#include <GL/glew.h>

#include "Denix/Core.h"

namespace Denix
{
	class VertexBuffer
	{
	public:
		VertexBuffer()
		{
			glGenBuffers(1, &m_GL_ID);

			if (m_GL_ID) DE_LOG(LogGL, Trace, "Created VBO ID: {}", m_GL_ID)
			else DE_LOG(LogGL, Error, "Failed to create VBO")
		}

		~VertexBuffer()
		{
			if(m_GL_ID) glDeleteBuffers(1, &m_GL_ID);
		}

		void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_GL_ID); }
		static void Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }


		void VertexBufferData(const GLsizei _size, const void* _data, 
		                const GLsizei _perPrimitive, const GLuint _count, const GLenum _type)
		{
			m_Size = _size;
			m_VertexPerPrimitive = _perPrimitive;
			m_Count = _count;
			m_Type = _type;
			glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);

			if(BufferSize()) DE_LOG(LogGL, Trace, "Uploaded data to VBO ID: {}", m_GL_ID)
			else DE_LOG(LogGL, Error, "Failed to upload data to VBO ID: {}", m_GL_ID)
		}

		GLsizei BufferSize() const
		{
			GLsizei size = 0;
			glBindBuffer(GL_ARRAY_BUFFER, m_GL_ID);
			glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			return size;
		}

		GLenum GetType() const { return m_Type; }

		GLsizei GetSize() const { return m_Size; }

		GLuint GetCount() const { return m_Count; }
		GLuint GetPerPrimitive() const { return m_VertexPerPrimitive; }

	private:
		GLuint m_GL_ID = 0;

		// Type of data stored in buffer, e.g. GL_FLOAT
		GLenum m_Type;

		// Number of vertices per primitive 1 - point, 3 - tri etc
		GLsizei m_VertexPerPrimitive;

		// Total number of vertices
		GLuint m_Count;

		// Size of data stored in buffer
		GLsizei m_Size;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer()
		{
			glGenBuffers(1, &m_GL_ID);

			if (m_GL_ID) DE_LOG(LogGL, Trace, "Created IBO ID: {}", m_GL_ID)
			else DE_LOG(LogGL, Error, "Failed to create IBO")
		}

		~IndexBuffer()
		{
			if (m_GL_ID) glDeleteBuffers(1, &m_GL_ID);
		}

		void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GL_ID); }
		void SetIndexCount(GLsizei _indexCount) { m_IndexCount = _indexCount; }
		static void Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }


		void IndexBufferData(const GLsizei _size, const void* _data,const GLuint _indexCount, const GLenum _type)
		{
			m_Size = _size;
			m_IndexCount = _indexCount;
			m_Type = _type;
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);

			if (BufferSize()) DE_LOG(LogGL, Trace, "Uploaded data to IBO ID: {}", m_GL_ID)
			else DE_LOG(LogGL, Error, "Failed to upload data to IBO ID: {}", m_GL_ID)
		}

		GLsizei BufferSize() const
		{
			GLsizei size = 0;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GL_ID);
			glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			return size;
		}

		GLenum GetType() const { return m_Type; }

		GLsizei GetSize() const { return m_Size; }

		GLuint GetIndexCount() const { return m_IndexCount; }

	private:
		GLuint m_GL_ID = 0;

		// Type of data stored in buffer, e.g. GL_FLOAT
		GLenum m_Type = GL_UNSIGNED_INT;

		// Total number of indexes
		GLuint m_IndexCount;

		// Size of data stored in buffer
		GLsizei m_Size;
	};
}