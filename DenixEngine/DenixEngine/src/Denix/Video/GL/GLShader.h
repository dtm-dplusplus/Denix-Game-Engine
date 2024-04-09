#pragma once
#include <GL/glew.h>

#include "Denix/Core.h"
#include "Denix/Scene/Object.h"
#include "Denix/System/FileSubsystem.h"

namespace Denix
{
	struct ShaderSource
	{
		GLenum Type = 0;
		std::string Path;
		std::string Source;
		bool IsCompiled = false;
	};

	class GLShader: public Object
	{
	public:
		GLShader(const ObjectInitializer& _objInit): Object(_objInit)
		{
			CreateProgram();
		}

		~GLShader() override
		{
			DeleteProgram();
		}

		void Bind() const { glUseProgram(m_GL_ID); }
		static void Unbind() { glUseProgram(0); }

		GLuint CreateProgram()
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

		void DeleteProgram() const
		{
			if (m_GL_ID)
			{
				DE_LOG(LogShader, Trace, "Deleted shader program ID: {}", m_GL_ID)
				glDeleteProgram(m_GL_ID);
			}
		}

		bool LinkProgram() const
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

		GLuint CompileShader(ShaderSource& _sourceObj) const
		{
			// We should error check this in the future
			_sourceObj.Source = FileSubsystem::Read(_sourceObj.Path, true);

			if (const GLuint shader = glCreateShader(_sourceObj.Type))
			{
				const char* src = _sourceObj.Source.c_str();
				glShaderSource(shader, 1, &src, NULL);

				glCompileShader(shader);

				GLint result;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
				if (!result)
				{
					GLchar infoLog[512];
					glGetShaderInfoLog(shader, 512, NULL, infoLog);
					DE_LOG(LogShader, Error, "GLShader Error: {}", infoLog)
					return 0;
				}

				glAttachShader(m_GL_ID, shader);
				return shader;
			}

			DE_LOG(LogShader, Error, "Failed to create shader\n")
			return 0;
		}

		bool CompileProgram()
		{
			for (ShaderSource& source : m_ShaderSources)
			{
				if (const GLuint shader = CompileShader(source))
				{
					source.IsCompiled = true;
					DE_LOG(LogShader, Trace, "Shader compiled successfully")
				}
				else
				{
					source.IsCompiled = false;
					DE_LOG(LogShader, Error, "Shader compilation failed")
					return false;
				}
			}

			if (!LinkProgram())
			{
				DeleteProgram();
				return false;
			}

			return true;
		}

		GLint GetUniform(const std::string& _uniform)
		{
			if (const GLint uniform = glGetUniformLocation(m_GL_ID, _uniform.c_str()); uniform != -1)
			{
				m_ShaderUniforms[_uniform] = uniform;
				//DE_LOG(LogShader, Trace, "Uniform {} found", _uniform)
				return uniform;
			}
			else
			{
				//DE_LOG(LogShader, Error, "Uniform {} not found", _uniform)
				return uniform;
			}
		}

		GLuint GetGL_ID() const { return m_GL_ID; }

		void SetShaderSources(const std::vector<ShaderSource>& _sources)
		{
			m_ShaderSources = _sources;
		}

	private:
		GLuint m_GL_ID;

		std::vector<ShaderSource> m_ShaderSources;
		std::unordered_map<std::string, GLint> m_ShaderUniforms;
	};
}
