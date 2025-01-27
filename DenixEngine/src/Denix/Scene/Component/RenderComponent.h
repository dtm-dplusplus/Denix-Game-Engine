#pragma once

#include <filesystem>


#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/Shader.h"
#include "Denix/Video/GL/Material.h"
#include "Denix/Video/GL/Texture.h"


namespace Denix
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent();
		~RenderComponent() override = default;
		
		Ref<Shader> GetShader() const { return m_Shader; }
		void SetShader(const Ref<Shader>& _shader) { m_Shader = _shader; }

		Ref<Material> GetMaterial() const { return m_Material; }
		void SetMaterial(const Ref<Material>& _material);

		bool m_IsUI = false;		
		bool m_IsVisible = true;

	private:
		void Serialize(YAML::Emitter& _out) override;
		void Deserialize(const YAML::Node& _in) override;

		Ref<Shader> m_Shader;
		Ref<Material> m_Material;

		friend class Actor;
		friend class SceneSubsystem;
		friend class RendererSubsystem;
		friend class UISubsystem;
	};
}
