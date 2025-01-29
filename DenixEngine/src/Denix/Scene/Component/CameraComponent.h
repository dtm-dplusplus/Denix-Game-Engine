#pragma once

#include "Denix/Scene/Component.h"
#include "Denix/Core/Math/Math.h"


namespace Denix
{
    class Viewport;
    class TransformComponent;
}

namespace Denix
{
    class CameraComponent : public Component
    {
    public:
        CameraComponent();
        ~CameraComponent() override = default;


        void ProcessKeyboardInput(float _deltaTime, const Ref<Counter>& _waitCounter);
        void ProccessMouseMovement(float _deltaTime, const Ref<Counter>& _waitCounter);

        glm::mat4 GetProjectionMatrix() const { return m_Projection; }

        glm::mat4& GetProjectionMatrix() { return m_Projection; }

        glm::mat4 GetViewMatrix() const { return m_View; }

        glm::mat4& GetViewMatrix() { return m_View; }

        Ref<Viewport> GetViewport() const { return m_Viewport; }

        bool m_ExternalControl = false;

        // Camera Properties
        bool m_IsPerspective = true;
        float m_Fov = 45.f;

        float m_NearPlane = 0.1f;
        float m_FarPlane = 10000.f;

        bool m_OrthoOverride = false;
        float m_OrthoBottom = -1.0f;
        float m_OrthoTop = 1.0f;
        float m_OrthoLeft = -1.0f;
        float m_OrthoRight = 1.0f;

        // Factor used to represent the speed of the rotation within respectable limits
        bool m_EnableRotation = true;
        float m_RotationFactor = 100.0f;
        float m_PitchRotationRate = 1.0f;
        float m_YawRotationRate = 1.0f;
        float m_MoveSpeed = 10.0f;
        float m_MouseScrollSpeed = 0.5f;

    private:
        void BeginScene() override;

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        bool m_IsGameCamera = false;
        glm::vec2 m_Aspect;
        float m_AspectRatio;
        // Viewport
        Ref<Viewport> m_Viewport;
        WRef<TransformComponent> m_TransformComponent;
        // Matrices
        glm::mat4 m_Projection;
        glm::mat4 m_View;

        friend class Engine;
        friend class RendererSubsystem;
        friend class EditorSubsystem;
        friend class Scene;
        friend class SceneSubsystem;
    };
}
