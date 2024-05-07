#pragma once

#include <filesystem>


#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/GLShader.h"
#include "Denix/Video/GL/Material.h"
#include "Denix/Video/GL/Texture.h"


namespace Denix
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent();

		RenderComponent(Ref<RenderComponent> _other);

		RenderComponent(const std::string& _parentName);

		~RenderComponent() override = default;

		Ref<Texture> GetTexture() const { return m_Texture; }
		void SetTexture(const Ref<Texture>& _texture) { m_Texture = _texture; }

		TextureSettings GetTextureSettings() const { return m_TextureSettings; }
		TextureSettings& GetTextureSettings() { return m_TextureSettings; }
		void SetTextureSettings(const TextureSettings& _settings) { m_TextureSettings = _settings; }

		Ref<GLShader> GetShader() const { return m_Shader; }
		void SetShader(const Ref<GLShader>& _shader) { m_Shader = _shader; }

		Ref<Material> GetMaterial() const { return m_Material; }
		void SetMaterial(const Ref<Material>& _material) { m_Material = _material; }
		

		glm::vec4 GetDebugColor() const { return m_DebugColor; }
		glm::vec4& GetDebugColor() { return m_DebugColor; }
		glm::vec4 SetDebugColor(const glm::vec4& _color) { return m_DebugColor = _color; }

		bool IsVisible() const { return m_IsVisible; }
		bool& IsVisible() { return m_IsVisible; }
		void SetIsVisible(const bool _visible) { m_IsVisible = _visible; }

		bool AffectsLighting() const { return m_AffectsLighting; }
		bool& AffectsLighting() { return m_AffectsLighting; }
		void SetAffectsLighting(const bool _affectsLighting) { m_AffectsLighting = _affectsLighting; }

		bool GetBaseColorAsTexture() const { return m_BaseColorAsTexture; }
		bool& GetBaseColorAsTexture() { return m_BaseColorAsTexture; }
		void SetBaseColorAsTexture(const bool _asTexture) { m_BaseColorAsTexture = _asTexture; }

	public:
		void BeginScene() override;

		void EndScene() override;

		void RegisterComponent() override;
		void UnregisterComponent() override;
		

	private:
		bool m_IsVisible = true;
		bool m_AffectsLighting = true;
		bool m_BaseColorAsTexture = false;

		glm::vec4 m_DebugColor = glm::vec4(0.98f, 1.f, 1.f, 1.f);
		Ref<GLShader> m_Shader;
		Ref<Texture> m_Texture;
		Ref<Material> m_Material;

		// Texture
		TextureSettings m_TextureSettings;
		

		friend class SceneSubsystem;
		friend class RendererSubsystem;
	};
}
