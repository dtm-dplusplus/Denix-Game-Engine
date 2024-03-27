#include "depch.h"
#include "GLShader.h"

namespace Denix
{
	GLuint GLShader::CreateProgram()
	{
		if (const GLuint program = glCreateProgram())
		{
			m_GL_ID = program;
			DE_LOG(LogShader, Trace, "Created shader program ID: {}", program)

			return program;
		}

		DE_LOG(LogShader, Error, "Failed to create shader program")
		return 0;
	}

	bool GLShader::LinkProgram() const
	{
		glLinkProgram(m_GL_ID);

		GLint result;
		glGetProgramiv(m_GL_ID, GL_LINK_STATUS, &result);
		if (!result)
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(m_GL_ID, 512, NULL, infoLog);
			DE_LOG(LogShader, Error, "GLShader Program Link Fail: {}", infoLog)
			return false;
		}

		DE_LOG(LogShader, Trace, "GLShader Program Link Success")

		return true;
	}
}