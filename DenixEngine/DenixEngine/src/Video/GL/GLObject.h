#pragma once

#include <GL/glew.h>

class GLObject
{
public:
	GLObject() : m_glID{ 0 } {}
	virtual ~GLObject() = default;

	GLuint GetID() const { return m_glID; }
protected:
	GLuint m_glID;
};