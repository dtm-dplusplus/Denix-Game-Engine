	#pragma once

#include "Actor.h"
#include "Component/CameraComponent.h"

namespace Denix
{
	class Camera : public Actor
	{
	public:
		/**
		 *  @brief Constructor for the Camera class
		 */
		Camera();

		~Camera() override = default;

		void Update(float _deltaTime) override;

		Ref<CameraComponent> GetCameraComponent() const { return m_CameraComponent; }
	
	private:
		Ref<CameraComponent> m_CameraComponent;

		friend class Engine;
		friend class RendererSubsystem;
		friend class Scene;
		friend class SceneSubsystem;
	};
}