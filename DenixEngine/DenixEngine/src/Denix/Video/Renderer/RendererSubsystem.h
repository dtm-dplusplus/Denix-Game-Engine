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

		static void DrawImmediate(
			const Ref<RenderComponent>& _renderComp, 
			const Ref<TransformComponent>& _transformComp, 
			const Ref<MeshComponent>& _meshComp);
		
		static int& GetViewportMode() { return s_RendererSubSystem->m_ViewportMode; }


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

		void SetActiveScene(const Ref<Scene>& _scene);

		static RendererSubsystem* s_RendererSubSystem;
		
		std::vector<std::tuple<
			Ref<RenderComponent>, 
			Ref<TransformComponent>, 
			Ref<MeshComponent>>> m_RendererComponents;

		Ref<Scene> m_ActiveScene;

		int m_ViewportMode = static_cast<int>(ViewportMode::Default);

		friend class SceneSubsystem;
	};
}

#pragma once