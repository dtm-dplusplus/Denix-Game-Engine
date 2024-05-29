#pragma once

#include <filesystem>


#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/Shader.h"
#include "Denix/Video/GL/Material.h"
#include "Denix/Video/GL/Texture.h"


namespace Denix
{
	enum class ViewportMode
	{
		Default,
		Unlit,
		Wireframe,
		Collision
	};

	class RenderComponent : public Component
	{
	public:
		RenderComponent();

		RenderComponent(Ref<RenderComponent> _other);

		RenderComponent(const std::string& _parentName);

		~RenderComponent() override = default;

		TextureSettings GetTextureSettings() const { return m_TextureSettings; }
		TextureSettings& GetTextureSettings() { return m_TextureSettings; }
		void SetTextureSettings(const TextureSettings& _settings) { m_TextureSettings = _settings; }

		Ref<Material> GetMaterial() const { return m_Material; }
		void SetMaterial(const Ref<Material>& _material) { m_Material = _material; }
		

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
		

	private:
		bool m_IsVisible = true;
		bool m_AffectsLighting = true;

		Ref<Material> m_Material;

		// Texture
		TextureSettings m_TextureSettings;
		

		friend class SceneSubsystem;
		friend class RendererSubsystem;
	};
}
