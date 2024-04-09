#pragma once
#include "Subsystem.h"
#include "Denix/Video/GL/GLShader.h"

#include "FileSubsystem.h"
#include "Denix/Core.h"
#include "Denix/Scene/Scene.h"

namespace Denix
{
	/**
	 * \ Responsible for managing shaders
	 */
	class ShaderSubsystem: public Subsystem
	{
	public:
		ShaderSubsystem()
		{
			s_ShaderSubSystem = this;

			DE_LOG_CREATE(LogShaderSubSystem)
		}

		~ShaderSubsystem() override
		{
			s_ShaderSubSystem = nullptr;
		}

		static ShaderSubsystem* Get() { return s_ShaderSubSystem; }

		void Initialize() override
		{
			DE_LOG(Log, Trace, "GLShader Subsystem Initialized")

			// Create Debug Shader
			{
				std::vector<ShaderSource> shaders;
				shaders.emplace_back(FileSubsystem::GetEngineRoot() + R"(res\shaders\DebugVertex.glsl)");
				shaders.emplace_back(FileSubsystem::GetEngineRoot() + R"(res\shaders\DebugFragment.glsl)");
				LoadShader(shaders, "DebugShader");
			}

			// Create Texture Shader
			{
				std::vector<ShaderSource> shaders;
				shaders.emplace_back(FileSubsystem::GetEngineRoot() + R"(res\shaders\Vertex.glsl)");
				shaders.emplace_back(FileSubsystem::GetEngineRoot() + R"(res\shaders\Fragment.glsl)");
				LoadShader(shaders, "DefaultShader");
			}
			m_Initialized = true;
		}

		void Deinitialize() override
		{
			DE_LOG(Log, Trace, "GLShader Subsystem Initialized")
			m_Initialized = false;
		}
		
		void AddShader(const Ref<GLShader>& _shader)
		{
			if (ShaderExists(_shader->GetFriendlyName()))
			{
				DE_LOG(LogShader, Error, "GLShader already exists: {}", _shader->GetFriendlyName())
				return;
			}

			m_ShaderPrograms[_shader->GetFriendlyName()] = _shader;
		}
		bool LoadShader(const std::vector<ShaderSource>& _shaders, const std::string& _name)
		{
			if (const Ref<GLShader> program = MakeRef<GLShader>(ObjectInitializer(_name)))
			{
				if (!program->GetGL_ID()) return false;

				program->SetShaderSources(_shaders);

				if (!program->CompileProgram()) return false;

				AddShader(program);

				return true;
			}
			
			return false;
		}

		Ref<GLShader> GetShader(const std::string& _name)
		{
			if(ShaderExists(_name))
			{
				return m_ShaderPrograms[_name];
			}

			DE_LOG(LogShader, Error, "GLShader not found: {}", _name)
			return nullptr;
		}

		bool ShaderExists(const std::string& _name) const
		{
			return m_ShaderPrograms.contains(_name);
		}
	private:
		static ShaderSubsystem* s_ShaderSubSystem;

		std::unordered_map<std::string, Ref<GLShader>> m_ShaderPrograms;
	};
}
