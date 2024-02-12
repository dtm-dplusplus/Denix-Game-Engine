#pragma once

#include "GLObject.h"
#include "DeLog.h"

class VertexBuffer final : public GLObject
{
public:
	VertexBuffer() = default;
	VertexBuffer(const void* _data, const GLuint _size)
	{
		GenVertexBuffer();
		Bind();
		BufferData(_data, _size);
		Unbind();
	}

	~VertexBuffer() override
	{
		glDeleteBuffers(1, &m_glID);

		if (!m_glID) DE_LOG(LogGL, Trace, "Deleted VBO ID: {}", m_glID)
		else DE_LOG(LogGL, Error, "Failed to delete VBO")
	}

	void GenVertexBuffer() 
	{
		glGenBuffers(1, &m_glID);

		if (m_glID) DE_LOG(LogGL, Trace, "Created VBO ID: {}", m_glID)
		else DE_LOG(LogGL, Error, "Failed to create VBO")
	}

	void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_glID); }
	static void Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

	void BufferData(const void* _data, const GLuint _size) 
	{ 
		glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW); 

		if(BufferSize()) DE_LOG(LogGL, Trace, "Uploaded data to VBO ID: {}", m_glID)
		else DE_LOG(LogGL, Error, "Failed to upload data to VBO ID: {}", m_glID)
	}

	GLuint BufferSize() const
	{
		GLint size = 0;
		glBindBuffer(GL_ARRAY_BUFFER, m_glID);
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return size;
	}
};