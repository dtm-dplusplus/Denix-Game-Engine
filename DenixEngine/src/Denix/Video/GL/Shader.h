#pragma once

#include <GL/glew.h>

#include "Denix/Core.h"
#include "Denix/Asset/Asset.h"

namespace Denix
{
	struct ShaderSource
	{
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

	constexpr std::array<std::pair<const char*, GLenum>, 6> g_SHADER_TYPES = {{
		{"vertex", GL_VERTEX_SHADER},
		{"fragment", GL_FRAGMENT_SHADER},
		{"geometry", GL_GEOMETRY_SHADER},
		{"tess_control", GL_TESS_CONTROL_SHADER},
		{"tess_evaluation", GL_TESS_EVALUATION_SHADER},
		{"compute", GL_COMPUTE_SHADER}
	}};

	constexpr std::string GetShaderTypeToString(const GLenum _type)
	{
		for (const auto& [stringType, enumType] : g_SHADER_TYPES) {
			if (_type == enumType) return stringType;
		}
		
		return "";
	}

	constexpr GLenum GetShaderTypeFromString(const std::string& _type) {
		for (const auto& [stringType, enumType] : g_SHADER_TYPES) {
			if (_type == stringType)  return enumType;
		}
		
		return GL_FALSE;
	}
	
	class Shader: public Asset
	{
	public:
		Shader()
		{
			CreateProgram();
		}


		~Shader() override
		{
			if (m_DeleteOnDestroy) DeleteProgram();
		}

		void Bind() const { glUseProgram(m_GL_ID); }
		static void Unbind() { glUseProgram(0); }

		

		GLuint GetGL_ID() const { return m_GL_ID; }

		std::vector<ShaderSource>& GetShaderSources() { return m_ShaderSources; }

		
		
	private:
		GLuint CreateProgram();

		void DeleteProgram() const;

		bool LinkProgram() const;

		bool CompileShader(ShaderSource& _sourceObj) const;

		GLenum GetShaderType(const std::string& _source) const;

		bool CompileProgram();
		bool RecompileProgram();
		GLint GetUniform(const std::string& _uniform);

		GLuint m_GL_ID;

		/** If true, the shader will be deleted on destruction. Useful for Recompilation where we transfer GL ID */
		bool m_DeleteOnDestroy;
		
		std::vector<ShaderSource> m_ShaderSources;
		std::unordered_map<std::string, GLint> m_ShaderUniforms;
		
		friend class AssetSubsystem;
		friend class RendererSubsystem;
	};
}
