#pragma once

#include "GLObject.h"
#include "DeLog.h"

class VertexArray final : public GLObject
{
public:
	VertexArray() = default;

	~VertexArray() override
	{
		glDeleteVertexArrays(1, &m_glID);

		if (!m_glID) DE_LOG(LogGL, Trace, "Deleted VAO ID: {}", m_glID)
		else DE_LOG(LogGL, Error, "Failed to delete")
	}

	void GenVertexArray() 
	{
		glGenVertexArrays(1, &m_glID);

		if (m_glID) DE_LOG(LogGL, Trace, "Created VAO ID: {}", m_glID)
		else DE_LOG(LogGL, Error, "Failed to create VAO")
	}

	void Bind() const { glBindVertexArray(m_glID); }
	static void Unbind() { glBindVertexArray(0); }

	void AttribPtr(const GLuint _index, const GLint _size, const GLenum _type,
	               const GLboolean _normalized = false,
	               const void* _ptr = (void*)0)
	{
		const GLsizei stride = _size * sizeof(_type);
		glVertexAttribPointer(_index, _size, _type, _normalized, stride, _ptr);
		glEnableVertexAttribArray(_index);
	}
};