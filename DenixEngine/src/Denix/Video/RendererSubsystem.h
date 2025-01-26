#pragma once

#include "Denix/Core/Subsystem.h"
#include "Denix/Core.h"

#include "Denix/Scene/Component/RenderComponent.h"

namespace Denix
{
	class Scene;
	class ModelComponent;
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

		Ref<Shader> m_UIShader;
	private:
		void Initialize() override;

		void Deinitialize() override;

		void RenderScene();
		void RenderDefaultViewport() const;

		static void SetActiveScene(const Ref<Scene>& _scene);

		WRef<Shader> m_DefaultShader;
		WRef<Scene> m_ActiveScene;

		friend class SceneSubsystem;
		friend class Engine;
	};
}

#pragma once