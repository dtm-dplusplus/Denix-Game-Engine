/**
 * @file CameraComponent.h
 * @brief Defines the CameraComponent class for managing camera properties and behavior.
 */

#pragma once

#include "Denix/Scene/Component.h"
#include "Denix/Core/Math/Math.h"
#include "Denix/Video/GL/Viewport.h"

namespace Denix
{
    class TransformComponent;
}

namespace Denix
{
    /**
     * @class CameraComponent
     * @brief A component that manages camera properties and transformations.
     *
     * This component handles the projection and view matrices, camera movement, and input processing.
     */
    class CameraComponent : public Component
    {
    public:
        /**
         * @brief Default constructor for CameraComponent.
         */
        CameraComponent();

        /**
         * @brief Default destructor for CameraComponent.
         */
        ~CameraComponent() override = default;

        /**
         * @brief Gets the projection matrix.
         * @return The projection matrix.
         */
        glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }

        /**
         * @brief Gets a reference to the projection matrix.
         * @return A reference to the projection matrix.
         */
        glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }

        /**
         * @brief Gets the view matrix.
         * @return The view matrix.
         */
        glm::mat4 GetViewMatrix() const { return m_View; }

        /**
         * @brief Gets a reference to the view matrix.
         * @return A reference to the view matrix.
         */
        glm::mat4& GetViewMatrix() { return m_View; }

        /**
         * @brief Gets the viewport associated with this camera.
         * @return A reference to the viewport.
         */
        Ref<Viewport> GetViewport() const { return m_Viewport; }

        /** @brief Flag indicating whether the camera is externally controlled. */
        bool m_ExternalControl = false;

        /** @brief Flag indicating whether the camera uses perspective projection. */
        bool m_IsPerspective = true;

        /** @brief Field of view for perspective projection. */
        float m_Fov = 45.f;

        /** @brief Near clipping plane distance. */
        float m_NearPlane = 0.1f;

        /** @brief Far clipping plane distance. */
        float m_FarPlane = 10000.f;

        /** @brief Flag indicating whether orthographic properties are overridden. */
        bool m_OrthoOverride = false;

        /** @brief Orthographic bottom boundary. */
        float m_OrthoBottom = -1.0f;

        /** @brief Orthographic top boundary. */
        float m_OrthoTop = 1.0f;

        /** @brief Orthographic left boundary. */
        float m_OrthoLeft = -1.0f;

        /** @brief Orthographic right boundary. */
        float m_OrthoRight = 1.0f;

        /** @brief Flag to enable/disable camera rotation. */
        bool m_EnableRotation = true;

        /** @brief Rotation speed factor. */
        float m_RotationFactor = 100.0f;

        /** @brief Pitch rotation rate. */
        float m_PitchRotationRate = 1.0f;

        /** @brief Yaw rotation rate. */
        float m_YawRotationRate = 1.0f;

        /** @brief Movement speed of the camera. */
        float m_MoveSpeed = 10.0f;

        /** @brief Mouse scroll speed factor. */
        float m_MouseScrollSpeed = 0.5f;

    private:
        /**
         * @brief Processes keyboard input for camera movement.
         * @param _deltaTime The time elapsed since the last frame.
         * @param _waitCounter A reference to a counter used for synchronization.
         */
        void ProcessKeyboardInput(float _deltaTime, const Ref<Counter>& _waitCounter);

        /**
         * @brief Processes mouse movement for camera rotation.
         * @param _deltaTime The time elapsed since the last frame.
         * @param _waitCounter A reference to a counter used for synchronization.
         */
        void ProccessMouseMovement(float _deltaTime, const Ref<Counter>& _waitCounter);

        /**
         * @brief Prepares the camera at the beginning of a scene.
         */
        void BeginScene() override;

        /**
         * @brief Updates the camera state.
         * @param _deltaTime The time elapsed since the last frame.
         * @param _waitCounter A reference to a counter used for synchronization.
         */
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        glm::vec2 m_Aspect; ///< The aspect ratio of the window.
        float m_AspectRatio; ///< The aspect ratio value.
        glm::mat4 m_ProjectionMatrix; ///< The projection matrix of the camera.
        glm::mat4 m_View; ///< The view matrix of the camera.
        Ref<Viewport> m_Viewport; ///< The viewport associated with the camera.
        WRef<TransformComponent> m_TransformComponent; ///< Cached reference to the parent transform component.
    };
}