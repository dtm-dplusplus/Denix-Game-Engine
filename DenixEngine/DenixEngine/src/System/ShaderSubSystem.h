#pragma once
#include "SubSystem.h"
#include "Video/GL/GLShader.h"
#include "../Core.h"

/**
 * \ Responsible for managing shaders
 */
class ShaderSubSystem: public SubSystem
{
public:
	ShaderSubSystem()
	{
		s_ShaderSubSystem = this;

		DE_LOG_CREATE(LogShaderSubSystem)
	}

	~ShaderSubSystem() override
	{
		s_ShaderSubSystem = nullptr;
	}

	static ShaderSubSystem* Get() { return s_ShaderSubSystem; }

	void Initialize() override
	{
		DE_LOG(Log, Trace, "GLShader SubSystem Initialized")
		m_Initialized = true;

		// Create Debug Shader
		std::vector<std::pair<GLenum, std::string>> shaders;
		shaders.emplace_back(GL_VERTEX_SHADER, File::Read("res/shaders/UniVert.glsl"));
		shaders.emplace_back(GL_FRAGMENT_SHADER, File::Read("res/shaders/UniFrag.glsl"));
		LoadShader(shaders, "DebugShader");

		const Ref<GLShader> program = GetShader("DebugShader");
		
		// Check Uniforms
		program->GetUniform("u_Model");
		program->GetUniform("u_Projection");
		program->GetUniform("u_Color");
		program->GetUniform("u_View");


	}

	void Deinitialize() override
	{
		DE_LOG(Log, Trace, "GLShader SubSystem Initialized")
		m_Initialized = false;
	}

	bool LoadShader(const std::vector<std::pair<GLenum, std::string>>& _sources, const std::string& _name)
	{
		if(const Ref<GLShader> program = std::make_shared<GLShader>())
		{
			program->CreateProgram();

			for(const auto& [type, source] : _sources)
			{
				if (const GLuint shader = program->CompileShader(type, source))
				{
					program->AttachShader(shader);
					program->DeleteShader(shader);
				}
				else
				{
					program->DeleteProgram();
					return false;
				}
			}

			if (!program->LinkProgram())
			{
				program->DeleteProgram();
				return false;
			}

			m_ShaderPrograms[_name] = program;
			return true;
		}

		return false;
	}

	Ref<GLShader> GetShader(const std::string& _name)
	{
		if(m_ShaderPrograms.contains(_name))
		{
			return m_ShaderPrograms[_name];
		}

		DE_LOG(LogShader, Error, "GLShader not found: {}", _name)
		return nullptr;
	}
private:
	static ShaderSubSystem* s_ShaderSubSystem;

	std::unordered_map<std::string, Ref<GLShader>> m_ShaderPrograms;
};