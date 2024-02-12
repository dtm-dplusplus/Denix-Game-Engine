#pragma once
#include <GL/glew.h>

#include "GLObject.h"

// Shader class for debugging
class Shader: public GLObject
{
public:
	Shader() = default;
	Shader(const GLenum _type, const std::string_view _src)
	{
		m_glID = glCreateShader(_type);
		m_Type = _type;
		m_Source = _src.data();
	}

	// Deletes shader object. Will move this to destructor in future builds
	void Delete() const { glDeleteShader(m_glID); }
private:
	GLenum m_Type;

	std::string m_Source;

	friend class ShaderProgram;
};

class ShaderProgram: public GLObject
{
public:
	ShaderProgram() = default;

	~ShaderProgram() override
	{
		glDeleteProgram(m_glID);
	}

	void Bind() const { glUseProgram(m_glID); }
	static void Unbind() { glUseProgram(0); }

	GLuint CreateProgram();

	std::unique_ptr<Shader> CompileShader(GLenum _type, std::string_view _src);

	void AttachShader(const GLuint _shader) const { glAttachShader(m_glID, _shader); }
	void AttachShaders() const;

	void DetachShader(const GLuint _shader) const { glDetachShader(m_glID, _shader); }
	void DetachShaders() const;

	void BindAttrib(const GLuint _index, const std::string_view _name) const { glBindAttribLocation(m_glID, _index, _name.data()); }

	bool LinkProgram() const;

private:

	// Keep this for debugging (use GLints in future). Maybe #IF DEBUG - remove from release builds
	std::vector<std::unique_ptr<Shader>> m_Shaders;
};