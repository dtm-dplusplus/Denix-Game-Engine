#pragma once

#include "GLObject.h"
#include "Core.h"

class VertexArray final : public GLObject
{
public:
	VertexArray() = default;

	~VertexArray() override
	{
		if(m_glID) glDeleteVertexArrays(1, &m_glID);
	}

	void GenVertexArray() 
	{
		glGenVertexArrays(1, &m_glID);

		if (m_glID) DE_LOG(LogGL, Trace, "Created VAO ID: {}", m_glID)
		else DE_LOG(LogGL, Error, "Failed to create VAO")
	}

	void Bind() const { glBindVertexArray(m_glID); }
	static void Unbind() { glBindVertexArray(0); }

	void AttribPtr(const GLsizei _size, const GLenum _type,
	               const GLboolean _normalized = false,
	               const void* _ptr = (void*)0)
	{
		m_Stride += _size * sizeof(_type);
		glVertexAttribPointer(m_Index, _size, _type, _normalized, m_Stride, _ptr);
		glEnableVertexAttribArray(m_Index++); // Increment index for next attrib
	}

private:
	GLsizei m_Stride = 0;
	GLuint m_Index = 0;
};