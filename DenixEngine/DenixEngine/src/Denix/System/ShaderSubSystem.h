#pragma once
#include "Subsystem.h"
#include "Denix/Video/GL/GLShader.h"
#include "../Core.h"
#include "Denix/Core/File.h"

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
				std::vector<std::pair<GLenum, std::string>> shaders;
				shaders.emplace_back(GL_VERTEX_SHADER, File::Read("res\\shaders\\Vertex.glsl"));
				shaders.emplace_back(GL_FRAGMENT_SHADER, File::Read("res\\shaders\\Fragment.glsl"));
				LoadShader(shaders, "DebugShader");

				const Ref<GLShader> program = GetShader("DebugShader");

				// Check Uniforms
				program->GetUniform("u_Model");
				program->GetUniform("u_Projection");
				program->GetUniform("u_Color");
				program->GetUniform("u_View");
			}
			

			// Create Texture Shader
			{
				std::vector<std::pair<GLenum, std::string>> shaders;
				shaders.emplace_back(GL_VERTEX_SHADER, File::Read("res\\shaders\\TexVert.glsl"));
				shaders.emplace_back(GL_FRAGMENT_SHADER, File::Read("res\\shaders\\TexFrag.glsl"));
				LoadShader(shaders, "TextureShader");

				const Ref<GLShader> program = GetShader("TextureShader");

				// Check Uniforms
				program->GetUniform("u_Model");
				program->GetUniform("u_Projection");
				program->GetUniform("u_View");
				program->GetUniform("u_Texture");
			}
			

			m_Initialized = true;
		}

		void Deinitialize() override
		{
			DE_LOG(Log, Trace, "GLShader Subsystem Initialized")
			m_Initialized = false;
		}

		bool LoadShader(const std::vector<std::pair<GLenum, std::string>>& _sources, const std::string& _name)
		{
			if(const Ref<GLShader> program = MakeRef<GLShader>())
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
		static ShaderSubsystem* s_ShaderSubSystem;

		std::unordered_map<std::string, Ref<GLShader>> m_ShaderPrograms;
	};
}
