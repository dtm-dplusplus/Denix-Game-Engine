#pragma once

#include "Denix/System/Subsystem.h"
#include "Denix/Core.h"

#include "Denix/Video/Renderer/RenderComponent.h"

namespace Denix
{
	class Scene;
	class MeshComponent;
	class TransformComponent;
	class Camera;

	/** Manages Rendering of objects. Will move to component based submission instead of passing game object soon */
	class RendererSubsystem : public Subsystem
	{
	public:
		RendererSubsystem()
		{
			s_RendererSubSystem = this;
			DE_LOG_CREATE(LogRenderer)
		}

		~RendererSubsystem() override
		{
			s_RendererSubSystem = nullptr;
		}

		static int& GetViewportMode() { return s_RendererSubSystem->m_ViewportMode; }

		bool ValidateForRenderer(const Ref<RenderComponent>& _renderComp, const Ref<TransformComponent>& _transformComp, const Ref<MeshComponent>& _meshComp);

	public:
		static RendererSubsystem* Get() { return s_RendererSubSystem; }

		void Initialize() override
		{
			DE_LOG(LogRenderer, Trace, "RendererSubsystem Initialized")
			m_Initialized = true;
		}

		void Deinitialize() override
		{
			DE_LOG(LogRenderer, Trace, "RendererSubsystem Deinitialized")
			m_Initialized = false;
		}


	private:
		void RenderScene();
		void RenderDefaultViewport();
		void UploadCameraMatrices(const Denix::Ref<Denix::Shader>& shader);
		void RenderUnlitViewport();
		void RenderWireframeViewport();
		void RenderCollisionViewport();

		void RenderLighting();
		void RenderCollider(const Ref<class PhysicsComponent>& _component);

		static void SetActiveScene(const Ref<Scene>& _scene);

		static RendererSubsystem* s_RendererSubSystem;
		
		std::vector<std::tuple<
			Ref<RenderComponent>, 
			Ref<TransformComponent>, 
			Ref<MeshComponent>>> m_RendererComponents;

		Ref<Scene> m_ActiveScene;

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