#pragma once

#include <filesystem>


#include "Denix/Scene/Component.h"
#include "Denix/System/ShaderSubsystem.h"
#include "Denix/Video/GL/GLShader.h"
#include "Denix/Video/GL/Texture.h"


namespace Denix
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent() : Component(ObjectInitializer("Render Component"))
		{
			if (!m_Shader)
			{
				if (const Ref<GLShader> shader = ShaderSubsystem::Get()->GetShader("TextureShader"))
				{
					m_Shader = shader;
				}
			}

			// Load Default Texture
			std::string def = std::filesystem::current_path().parent_path().string() + "\\DenixEngine\\res\\Textures\\DefaultTexture.png";
			LoadTexture(def, "DefaultTexture");
		}

		RenderComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Render Component"))
		{
			if (!m_Shader)
			{
				if (const Ref<GLShader> shader = ShaderSubsystem::Get()->GetShader("TextureShader"))
				{
					m_Shader = shader;
				}
			}

			// Load Default Texture
			std::string def = std::filesystem::current_path().parent_path().string() + "\\DenixEngine\\res\\Textures\\DefaultTexture.png";
			LoadTexture(def, "DefaultTexture");
		}

		~RenderComponent() override = default;

		void LoadTexture(const std::string& _path, const std::string& _name)
		{
			m_Texture = MakeRef<Texture>(_path, _name);
			if (!m_Texture->LoadTexture()) return;
            m_TextureSettings = m_Texture->GetSettings();
		}

		Ref<Texture> GetTexture() const { return m_Texture; }

		enum class DrawMode
		{
			POINTS = GL_POINTS,
			LINES = GL_LINES,
			LINE_LOOP = GL_LINE_LOOP,
			LINE_STRIP = GL_LINE_STRIP,
			TRIANGLES = GL_TRIANGLES
			/*TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
			TRIANGLE_FAN = GL_TRIANGLE_FAN*/
		};

		Ref<GLShader> GetShader() const { return m_Shader; }
		void SetShader(const Ref<GLShader>& _shader) { m_Shader = _shader; }

		glm::vec4 GetDebugColor() const { return m_DebugColor; }
		glm::vec4& GetDebugColor() { return m_DebugColor; }
		glm::vec4 SetDebugColor(const glm::vec4& _color) { return m_DebugColor = _color; }

		GLenum GetDrawMode() const { return static_cast<GLenum>(m_DrawMode); }
		int& GetDrawMode() { return m_DrawMode; }
		void SetDrawMode(const GLenum _mode) { m_DrawMode = _mode; }

		bool IsVisible() const { return m_IsVisible; }
		bool& IsVisible() { return m_IsVisible; }
		void SetIsVisible(const bool _visible) { m_IsVisible = _visible; }

		bool AffectsLighting() const { return m_AffectsLighting; }
		bool& AffectsLighting() { return m_AffectsLighting; }
		void SetAffectsLighting(const bool _affectsLighting) { m_AffectsLighting = _affectsLighting; }

	public:
		void BeginScene() override;

		void EndScene() override;

		void RegisterComponent() override;
		void UnregisterComponent() override;
		
		TextureSettings& GetTextureSettings() { return m_TextureSettings; }

	private:
		bool m_IsVisible = true;
		bool m_AffectsLighting = true;

		glm::vec4 m_DebugColor = glm::vec4(0.98f, 1.f, 1.f, 1.f);
		Ref<GLShader> m_Shader;
		Ref<Texture> m_Texture;

		// Null effect atm as we are overriding with glPolygonMode globally
		int m_DrawMode = GL_TRIANGLES;

		// Texture
		TextureSettings m_TextureSettings;
		

		friend class SceneSubsystem;
		friend class RendererSubsystem;
	};
}
