#pragma once

#include "Subsystem.h"
#include "Denix/Core.h"
#include "Denix/Core/Logger.h"


namespace Denix
{
	class Scene;
	class MeshComponent;
	class TransformComponent;
	class RenderComponent;
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

		void DrawImmediate(
			const Ref<RenderComponent>& _render, 
			const Ref<TransformComponent>& _transform, 
			const Ref<MeshComponent>& _mesh);
			
		void SetActiveScene(const Ref<Scene>& _scene);

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
		static RendererSubsystem* s_RendererSubSystem;
		
		std::vector<std::tuple<
			Ref<RenderComponent>, 
			Ref<TransformComponent>, 
			Ref<MeshComponent>>> m_RendererComponents;

		Ref<Scene> m_ActiveScene;
	};
}

#pragma once