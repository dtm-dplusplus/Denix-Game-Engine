#pragma once
#include "SubSystem.h"
#include "Video/GL/Shader.h"
#include "../Core.h"

/**
 * \brief 
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
		DE_LOG(Log, Trace, "Shader SubSystem Initialized")
		m_Initialized = true;

		// Create ShaderProgram
		std::vector<std::pair<GLenum, std::string>> shaders;
		shaders.emplace_back(GL_VERTEX_SHADER, File::Read("res/shaders/UniVert.glsl"));
		shaders.emplace_back(GL_FRAGMENT_SHADER, File::Read("res/shaders/UniFrag.glsl"));
		LoadShader(shaders, "Shader");
	}

	void Deinitialize() override
	{
		DE_LOG(Log, Trace, "Shader SubSystem Initialized")
		m_Initialized = false;
	}

	bool LoadShader(const std::vector<std::pair<GLenum, std::string>>& _sources, const std::string& _name)
	{
		if(const Ref<ShaderProgram> Program = std::make_shared<ShaderProgram>())
		{
			Program->CreateProgram();

			for(const auto& [type, source] : _sources)
			{
				Program->CompileShader(type, source);
			}

			Program->AttachShaders();

			Program->BindAttrib(0, "a_Position");
			Program->LinkProgram();

			// Check Uniforms
			Program->GetUniform("u_Model");
			Program->GetUniform("u_Projection");
			Program->GetUniform("u_Color");
			Program->GetUniform("u_View");

			m_ShaderPrograms[_name] = Program;
			return true;
		}
	}

	Ref<ShaderProgram> GetShader(const std::string& _name)
	{
		if(m_ShaderPrograms.contains(_name))
		{
			return m_ShaderPrograms[_name];
		}

		DE_LOG(LogShader, Error, "Shader not found: {}", _name)
		return nullptr;
	}
private:
	static ShaderSubSystem* s_ShaderSubSystem;

	std::unordered_map<std::string, Ref<ShaderProgram>> m_ShaderPrograms;
};