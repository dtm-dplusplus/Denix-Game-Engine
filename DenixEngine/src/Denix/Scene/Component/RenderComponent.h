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

		~RenderComponent() override = default;

		Ref<Texture> GetTexture() const { return m_Texture; }
		void SetTexture(const Ref<Texture>& _texture) { m_Texture = _texture; }

		TextureSettings GetTextureSettings() const { return m_TextureSettings; }
		TextureSettings& GetTextureSettings() { return m_TextureSettings; }
		void SetTextureSettings(const TextureSettings& _settings) { m_TextureSettings = _settings; }

		Ref<Shader> GetShader() const { return m_Shader; }
		void SetShader(const Ref<Shader>& _shader) { m_Shader = _shader; }

		Ref<Material> GetMaterial() const { return m_Material; }
		Ref<Material>& GetMaterial() { return m_Material; }
		void SetMaterial(const Ref<Material>& _material);

		bool IsVisible() const { return m_IsVisible; }
		bool& IsVisible() { return m_IsVisible; }
		void SetIsVisible(const bool _visible) { m_IsVisible = _visible; }

		bool AffectsLighting() const { return m_AffectsLighting; }
		bool& AffectsLighting() { return m_AffectsLighting; }
		void SetAffectsLighting(const bool _affectsLighting) { m_AffectsLighting = _affectsLighting; }

		bool m_IsUI = false;		

	private:
		void BeginScene() override;

		void EndScene() override;

		void RegisterComponent() override;
		void UnregisterComponent() override;
		
		bool m_IsVisible = true;
		bool m_AffectsLighting = true;

		Ref<Shader> m_Shader;
		Ref<Texture> m_Texture;
		Ref<Material> m_Material;

		// Texture
		TextureSettings m_TextureSettings;
		

		friend class SceneSubsystem;
		friend class RendererSubsystem;
	};
}
