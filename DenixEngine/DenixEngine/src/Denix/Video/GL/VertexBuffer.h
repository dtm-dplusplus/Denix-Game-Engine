#pragma once

#include "GLObject.h"
#include "Denix/Core.h"

namespace Denix
{
	class VertexBuffer final : public GLObject
	{
	public:
		VertexBuffer()
		{
			GenVertexBuffer();
		}

		VertexBuffer(const GLenum _target, const GLsizei _size, const void* _data, 
			const GLsizei _perPrimitive, const GLuint _count, const GLenum _type)
		{
			GenVertexBuffer();
			Bind(_target);
			BufferData(_target, _size, _data, _perPrimitive, _count, _type);
			Unbind(_target);
		}

		~VertexBuffer() override
		{
			if(m_glID) glDeleteBuffers(1, &m_glID);
		}

		void GenVertexBuffer() 
		{
			glGenBuffers(1, &m_glID);

			if (m_glID) DE_LOG(LogGL, Trace, "Created VBO ID: {}", m_glID)
			else DE_LOG(LogGL, Error, "Failed to create VBO")
		}

		void Bind(GLenum _target) const { glBindBuffer(_target, m_glID); }
		static void Unbind(GLenum _target) { glBindBuffer(_target, 0); }


		void BufferData(const GLenum _target, const GLsizei _size, const void* _data, 
		                const GLsizei _perPrimitive, const GLuint _count, const GLenum _type)
		{
			m_Target = _target;
			m_Size = _size;
			m_VertexPerPrimitive = _perPrimitive;
			m_Count = _count;
			m_Type = _type;
			glBufferData(_target, _size, _data, GL_STATIC_DRAW);

			if(BufferSize(_target)) DE_LOG(LogGL, Trace, "Uploaded data to VBO ID: {}", m_glID)
			else DE_LOG(LogGL, Error, "Failed to upload data to VBO ID: {}", m_glID)
		}

		GLsizei BufferSize(GLenum _target) const
		{
			GLsizei size = 0;
			glBindBuffer(_target, m_glID);
			glGetBufferParameteriv(_target, GL_BUFFER_SIZE, &size);
			glBindBuffer(_target, 0);
			return size;
		}

		GLenum GetType() const { return m_Type; }

		GLuint GetTarget() const { return m_Target; }

		GLsizei GetSize() const { return m_Size; }

		GLuint GetCount() const { return m_Count; }
		GLuint GetPerPrimitive() const { return m_VertexPerPrimitive; }

	private:

		// Type of data stored in buffer, e.g. GL_FLOAT
		GLenum m_Type;

		// Target of buffer, e.g. GL_ARRAY_BUFFER
		GLenum m_Target;

		// Number of vertices per primitive 1 - point, 3 - tri etc
		GLsizei m_VertexPerPrimitive;

		// Total number of vertices
		GLuint m_Count;

		// Size of data stored in buffer
		GLsizei m_Size;
	};
}