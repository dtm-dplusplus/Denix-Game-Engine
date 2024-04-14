#pragma once
#include <iostream>
#include <map>
#include <GL/glew.h>

#include "Denix/Core.h"
#include "Denix/Scene/Object.h"
#include "Denix/Core/FileSubsystem.h"

namespace Denix
{
	struct ShaderSource
	{
	public:
		ShaderSource(const std::string& _path)
		{
			Path = _path;

			std::string n;

			auto it = Path.find_last_of('\\');
			FileName = Path.substr(it + 1);
		}

		std::string Path;
		std::string FileName;
		std::string Source;
		GLenum Type = 0;
		bool IsCompiled = false;
	};

	/*struct ShaderTypePair
	{
		std::string StringType;
		GLenum EnumType;
	};*/

	// Keyword to find the shader type in the shader file
	const std::string g_SHADER_KEYWORD{ "DE_SHADER" };
	const int g_SHADER_KEYWORD_OFFSET = 1 + g_SHADER_KEYWORD.length();

	const std::tuple<std::string, GLenum> g_SHADER_TYPES[] =
	{
		{ "vertex", GL_VERTEX_SHADER },
		{ "fragment", GL_FRAGMENT_SHADER },
		{ "geometry", GL_GEOMETRY_SHADER },
		{ "tess_control", GL_TESS_CONTROL_SHADER },
		{ "tess_evaluation", GL_TESS_EVALUATION_SHADER },
		{ "compute", GL_COMPUTE_SHADER }
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

		bool CompileShader(ShaderSource& _sourceObj) const
		{
			// Check Shader Source
			if (const std::string& source = FileSubsystem::Read(_sourceObj.Path, true); !source.empty())
			{
				_sourceObj.Source = source;
			}
			else
			{
				DE_LOG(LogShader, Error, "Failed to read shader file: {}", _sourceObj.Path)
				return false;
			}

			// Get Shader Type
			if (const GLenum type = GetShaderType(_sourceObj.Source); type != GL_FALSE)
			{
				_sourceObj.Type = type;
			}
			else
			{
				DE_LOG(LogShader, Error, "Failed to get shader type")
				return false;
			}

			// Compile Shader
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
					return false;
				}

				glAttachShader(m_GL_ID, shader);
				glDeleteShader(shader);
				return true;
			}

			DE_LOG(LogShader, Error, "Failed to create shader\n")
			return false;
		}

		GLenum GetShaderType(const std::string& _source) const
		{
			if (const auto keywordIt = _source.find_first_of(g_SHADER_KEYWORD))
			{
				std::string type;

				for (auto typeIt = keywordIt + g_SHADER_KEYWORD_OFFSET;
					_source[typeIt] != ' ' && _source[typeIt] != '\n'; typeIt++)
				{
					type += _source[typeIt];
				}

				DE_LOG(LogShader, Trace, "Shader Type: {}", type)
				
				// Return the type if we find it
				for (const auto& [StringType, EnumType] : g_SHADER_TYPES)
					if (type == StringType) return EnumType;

			}

			DE_LOG(LogShader, Error, "Shader Keyword DE_SHADER not found or invalid type")

			return GL_FALSE;
		}

		bool CompileProgram()
		{
			for (ShaderSource& source : m_ShaderSources)
			{
				if (const GLuint shader = CompileShader(source))
				{
					source.IsCompiled = true;
				}
				else
				{
					source.IsCompiled = false;
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

		std::vector<ShaderSource>& GetShaderSources() { return m_ShaderSources; }
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
