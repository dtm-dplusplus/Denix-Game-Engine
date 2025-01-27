#pragma once

#include "RenderSubmission.h"
#include "Denix/Core/Subsystem.h"

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

		static void RenderObject(const RenderSubmission& _submission);
		static void SubmitCamera(const CameraSubmission& _submission);
	private:
		void Initialize() override;

		void Deinitialize() override;

		void RenderScene();
		void RenderDefaultViewport() const;

		CameraSubmission m_CameraSubmission;
		static void SetActiveScene(const Ref<Scene>& _scene);

		WRef<Shader> m_DefaultShader;
		WRef<Scene> m_ActiveScene;

		friend class SceneSubsystem;
		friend class Engine;
	};
}