#pragma once

#include "SubSystem.h"
#include "Denix/Core.h"
#include "Denix/Core/Logger.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Scene/Component/MeshComponent.h"
#include "Denix/Scene/Component/RenderComponent.h"

namespace Denix
{
	class Camera;

	/** Manages Rendering of objects. Will move to component based submission instead of passing game object soon */
	class RendererSubSystem : public SubSystem
	{
	public:
		RendererSubSystem()
		{
			s_RendererSubSystem = this;

			DE_LOG_CREATE(LogRenderer)
		}

		~RendererSubSystem() override
		{
			s_RendererSubSystem = nullptr;
		}

		void Draw();

		void Submit(const Ref<RenderComponent>& _render, const Ref<TransformComponent>& _transform, const Ref<MeshComponent>& _mesh);
			

	public:
		static RendererSubSystem* Get() { return s_RendererSubSystem; }

		void Initialize() override
		{
			DE_LOG(LogRenderer, Trace, "RendererSubSystem Initialized")

			m_Initialized = true;
		}

		void Deinitialize() override
		{
			DE_LOG(LogRenderer, Trace, "RendererSubSystem Deinitialized")
			m_Initialized = false;
		}

		void SetActiveCamera(const Ref<Camera>& _camera);

	private:
		static RendererSubSystem* s_RendererSubSystem;

		
		std::vector<std::tuple<
			Ref<RenderComponent>, 
			Ref<TransformComponent>, 
			Ref<MeshComponent>>> m_RendererComponents;

		Ref<Camera> m_ActiveCamera;
	};
}

#pragma once