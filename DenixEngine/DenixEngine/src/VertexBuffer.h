#pragma once

#include "GLObject.h"
#include "DeLog.h"

class VertexBuffer final : public GLObject
{
public:
	VertexBuffer() = default;
	VertexBuffer(const void* _data, const GLuint _size)
	{
		GenGlID();
		Bind();
		BufferData(_data, _size);
	}

	~VertexBuffer() override { DeleteGlID(); }

	void GenGlID() override 
	{
		glGenBuffers(1, &m_GlID);

		if (m_GlID) DE_LOG(LogGL, Trace, "Created positions VBO ID: %i", m_GlID)
		else DE_LOG(LogGL, Error, "Failed to create positions VBO: %i", m_GlID)
	}
	void DeleteGlID() override
	{
		glDeleteBuffers(1, &m_GlID);

		if(!m_GlID) DE_LOG(LogGL, Trace, "Deleted VBO ID: %i", m_GlID)
		else DE_LOG(LogGL, Error, "Failed to delete VBO ID: %i", m_GlID)
	}

	void Bind() override { glBindBuffer(GL_ARRAY_BUFFER, m_GlID); }
	void Unbind() override { glBindBuffer(GL_ARRAY_BUFFER, 0); }

	void BufferData(const void* _data, const GLuint _size) 
	{ 
		glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW); 

		if(BufferSize()) DE_LOG(LogGL, Trace, "Uploaded data to VBO ID: %i", m_GlID)
		else DE_LOG(LogGL, Error, "Failed to upload data to VBO ID: %i", m_GlID)
	}
	GLuint BufferSize() const
	{
		GLint size = 0;
		glBindBuffer(GL_ARRAY_BUFFER, m_GlID);
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		return size;
	}
};

