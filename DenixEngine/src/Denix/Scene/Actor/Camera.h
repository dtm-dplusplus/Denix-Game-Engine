#pragma once

#include "Denix/Scene/Actor.h"
#include "Denix/Scene/Component/CameraComponent.h"

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

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        Ref<CameraComponent> GetCameraComponent() const { return m_CameraComponent; }

    private:
        Ref<CameraComponent> m_CameraComponent;

        friend class Engine;
        friend class RendererSubsystem;
        friend class Scene;
        friend class SceneSubsystem;
    };
}
