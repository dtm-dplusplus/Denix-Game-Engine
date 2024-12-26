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


	class Shader: public Object
	{
	public:
		Shader(const Shader& other)
			: Object(other),
			  m_GL_ID(other.m_GL_ID),
			  m_ShaderSources(other.m_ShaderSources),
			  m_ShaderUniforms(other.m_ShaderUniforms)
		{
		}

		Shader(Shader&& other) noexcept
			:m_GL_ID(other.m_GL_ID),
			  m_ShaderSources(std::move(other.m_ShaderSources)),
			  m_ShaderUniforms(std::move(other.m_ShaderUniforms)),
				Object(std::move(other))


			  
		{
		}

		Shader& operator=(const Shader& other)
		{
			if (this == &other)
				return *this;
			Object::operator =(other);
			m_GL_ID = other.m_GL_ID;
			m_ShaderSources = other.m_ShaderSources;
			m_ShaderUniforms = other.m_ShaderUniforms;
			return *this;
		}

		Shader& operator=(Shader&& other) noexcept
		{
			if (this == &other)
				return *this;
			Object::operator =(std::move(other));
			m_GL_ID = other.m_GL_ID;
			m_ShaderSources = std::move(other.m_ShaderSources);
			m_ShaderUniforms = std::move(other.m_ShaderUniforms);
			return *this;
		}

		Shader(const ObjectInit& _objInit): Object(_objInit)
		{
			CreateProgram();
		}

		~Shader() override
		{
			//DeleteProgram();
		}

		void Bind() const { glUseProgram(m_GL_ID); }
		static void Unbind() { glUseProgram(0); }

		

		GLuint GetGL_ID() const { return m_GL_ID; }

		std::vector<ShaderSource>& GetShaderSources() { return m_ShaderSources; }

	private:
		GLuint CreateProgram();

		void DeleteProgram() const;

		bool LinkProgram() const;

		// Compile Shader
		// set sourceFromPath to false if you want to receive the source from the ShaderSource object
		bool CompileShader(ShaderSource& _sourceObj) const;

		GLenum GetShaderType(const std::string& _source) const;

		bool CompileProgram();

		GLint GetUniform(const std::string& _uniform);

		GLuint m_GL_ID;

		std::vector<ShaderSource> m_ShaderSources;
		std::unordered_map<std::string, GLint> m_ShaderUniforms;

		friend class AssetSubsystem;
		friend class RendererSubsystem;
	};
}
