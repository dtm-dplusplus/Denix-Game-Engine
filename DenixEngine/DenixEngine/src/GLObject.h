#pragma once

#include <GL/glew.h>

class GLObject
{
public:
	GLObject() : m_GlID{ 0 } {}

	virtual ~GLObject() = default;

	virtual void GenGlID() = 0;
	GLuint GetGlID() const { return m_GlID; }
	virtual void DeleteGlID() = 0;

	virtual void Bind() = 0;
	virtual	void Unbind() = 0;

protected:
	GLuint m_GlID;
};

