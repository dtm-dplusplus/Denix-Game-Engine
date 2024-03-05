#pragma once
#include <GL/glew.h>

#include "GLObject.h"

// Shader class for debugging
class Shader: public GLObject
{
public:
	Shader() = default;
	Shader(const GLenum _type, const std::string& _src)
	{
		m_glID = glCreateShader(_type);
		m_Type = _type;
		m_Source = _src;
	}

	// Deletes shader object. Will move this to destructor in future builds
	void Delete() const
	{
		glDeleteShader(m_glID);
	}

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

	Ref<Shader> CompileShader(GLenum _type, const std::string& _src)
	{
		if (Ref<Shader> shader = std::make_shared<Shader>(_type, _src); shader->GetID())
		{
			// Create Source
			const char* src = shader->m_Source.c_str();
			glShaderSource(shader->m_glID, 1, &src, NULL);

			// Compile Shader
			glCompileShader(shader->m_glID);

			// Check for errors
			if(!ErrorCheck(shader->m_glID))
			{
				DE_LOG(LogShader, Trace, "Shader Compilation Error: {}", shader->m_glID)
				return nullptr;
			}

			m_Shaders.push_back(std::move(shader));
			return shader;
		}

		DE_LOG(LogShader, Error, "Failed to create shader\n")
		return nullptr;
	}

	void AttachShader(const GLuint _shader) const
	{
		glAttachShader(m_glID, _shader);
	}

	void AttachShaders() const;

	void DetachShader(const GLuint _shader) const
	{
		glDetachShader(m_glID, _shader);
	}

	void DetachShaders() const;

	void BindAttrib(const GLuint _index, const std::string_view _name) const
	{
		glBindAttribLocation(m_glID, _index, _name.data());
	}

	GLint GetUniform(const std::string& _uniform)
	{
		if (GLint uniform = glGetUniformLocation(m_glID, _uniform.c_str()); uniform != -1)
		{
			m_Uniforms.emplace_back(uniform, _uniform);
			DE_LOG(LogShader, Trace, "Uniform {} found", _uniform)
			return uniform;
		}
		else
		{
			DE_LOG(LogShader, Error, "Uniform {} not found", _uniform)
			return uniform;
		}
	}

	bool LinkProgram() const;

private:

	// Debug Variables
	std::vector<std::pair<GLint, std::string>> m_Uniforms;

	std::vector<Ref<Shader>> m_Shaders;

	bool ErrorCheck(GLuint _ID) const
	{
		GLint result;
		glGetShaderiv(_ID, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			GLchar infoLog[512];
			glGetShaderInfoLog(_ID, 512, NULL, infoLog);
			DE_LOG(LogShader, Error, "Shader Error: {}", infoLog)
			return false;
		}

		return true;
	}
};