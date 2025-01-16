#pragma once

#include "Denix/Core/Subsystem.h"
#include "Denix/Core.h"

#include "Denix/Scene/Component/RenderComponent.h"

namespace Denix
{
	class Scene;
	class MeshComponent;
	class TransformComponent;
	class Camera;

	/** Manages Rendering of objects. Will move to component based submission instead of passing game object soon */
	class RendererSubsystem : public Subsystem<RendererSubsystem>
	{
	public:
		RendererSubsystem() = default;

		~RendererSubsystem() override = default;

		RendererSubsystem(const RendererSubsystem& _other) = delete;
		RendererSubsystem(RendererSubsystem&& _other) noexcept = delete;
		RendererSubsystem& operator=(const RendererSubsystem& _other) = delete;
		RendererSubsystem& operator=(RendererSubsystem&& _other) noexcept = delete;

		static int& GetViewportMode() { return s_Instance->m_ViewportMode; }

	private:
		void Initialize() override;

		void Deinitialize() override;

		void RenderScene();
		void RenderDefaultViewport() const;

		static void SetActiveScene(const Ref<Scene>& _scene);

		WRef<Shader> m_DefaultShader;
		WRef<Scene> m_ActiveScene;

		glm::vec3 m_WireframeColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 m_StaticColliderColor = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_DynamicColliderColor = glm::vec3(1.000, 0.078, 0.576);

		float m_ColliderAmbientIntensity = 0.7f;
		int m_ViewportMode = static_cast<int>(ViewportMode::Default);

		friend class SceneSubsystem;
		friend class Engine;
	};
}

#pragma once