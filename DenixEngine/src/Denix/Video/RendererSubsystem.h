#pragma once

#include "Denix/Core/Subsystem.h"
#include "RenderSubmission.h"

namespace Denix
{
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

		CameraSubmission m_CameraSubmission;

		friend class SceneSubsystem;
		friend class Engine;
	};
}