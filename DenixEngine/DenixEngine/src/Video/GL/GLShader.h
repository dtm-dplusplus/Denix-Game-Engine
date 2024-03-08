#pragma once
#include <GL/glew.h>

#include "GLObject.h"
#include "Core.h"

namespace Denix
{
	class GLShader final : public GLObject
	{
	public:
		GLShader() = default;

		~GLShader() override = default;

		/*static Ref<GLShader> Create()
		{
			return std::make_shared<GLShader>();
		}*/

		void Bind() const { glUseProgram(m_glID); }
		static void Unbind() { glUseProgram(0); }

		GLuint CreateProgram();
		bool LinkProgram() const;

		void DeleteProgram() const
		{
			glDeleteProgram(m_glID);
		}

		GLuint CompileShader(GLenum _type, const std::string& _source)
		{
			if (const GLuint shader = glCreateShader(_type))
			{
				const char* src = _source.c_str();
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

				return shader;
			}

			DE_LOG(LogShader, Error, "Failed to create shader\n")
			return 0;
		}

		void AttachShader(const GLuint _shaderID) const
		{
			glAttachShader(m_glID, _shaderID);
		}


		void DetachShader(const GLuint _shaderID) const
		{
			glDetachShader(m_glID, _shaderID);
		}

		GLuint CreateShader(const GLenum _type) const
		{
			return glCreateShader(_type);
		}

		void DeleteShader(const GLuint _shaderID) const
		{
			glDeleteShader(_shaderID);
		}

		void BindAttrib(const GLuint _index, const std::string& _name) const
		{
			glBindAttribLocation(m_glID, _index, _name.data());
		}

	
		GLint GetUniform(const std::string& _uniform)
		{
			if (const GLint uniform = glGetUniformLocation(m_glID, _uniform.c_str()); uniform != -1)
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

		/** TEMP */
		// Color
		GLint ColorUniformId;

		//// Matrix
		//GLint ModelUniformId;
		//GLint ViewUniformId;
		//GLint ProjectionUniformId;

		private:
	

	private:


		std::unordered_map<std::string, GLint> m_ShaderUniforms;

	};
}