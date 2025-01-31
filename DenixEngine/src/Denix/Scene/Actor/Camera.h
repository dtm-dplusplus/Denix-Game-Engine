/**
 * @file Camera.h
 * @brief Definition of the Camera class
 */

#pragma once

#include "Denix/Scene/Actor.h"
#include "Denix/Scene/Component/CameraComponent.h"

namespace Denix
{
    /**
     * @class Camera
     * @brief Actor container for CameraComponent.
     *
     * This class represents a Camera actor, which contains a CameraComponent.
     * It is used to manage camera-related functionality within the scene.
     */
    class Camera : public Actor
    {
    public:
        /**
         * @brief Constructor for the Camera class.
         */
        Camera();

        /**
         * @brief Default destructor for Camera.
         */
        ~Camera() override = default;

        /**
         * @brief Gets the CameraComponent associated with this Camera.
         * @return A reference to the CameraComponent.
         */
        Ref<CameraComponent> GetCameraComponent() const { return m_CameraComponent; }

    protected:
        Ref<CameraComponent> m_CameraComponent; ///< Reference to the CameraComponent.

    private:
        friend class RendererSubsystem; ///< Allows RendererSubsystem to access private members.
        friend class Scene; ///< Allows Scene to access private members.
        friend class SceneSubsystem; ///< Allows SceneSubsystem to access private members.
    };
}