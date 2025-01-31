/**
 * @file RendererSubsystem.h
 * @brief Declaration of the RendererSubsystem class, responsible for managing rendering operations.
 */

#pragma once

#include "Denix/Core/Subsystem.h"
#include "RenderSubmission.h"

namespace Denix
{
    /**
     * @class RendererSubsystem
     * @brief Manages the rendering of objects within the engine.
     *
     * This subsystem handles rendering operations and will move to a batched system in the future.
     */
    class RendererSubsystem : public Subsystem<RendererSubsystem>
    {
    public:
        /**
         * @brief Default constructor.
         */
        RendererSubsystem() = default;

        /**
         * @brief Default destructor.
         */
        ~RendererSubsystem() override = default;

        RendererSubsystem(const RendererSubsystem& _other) = delete;
        RendererSubsystem(RendererSubsystem&& _other) noexcept = delete;
        RendererSubsystem& operator=(const RendererSubsystem& _other) = delete;
        RendererSubsystem& operator=(RendererSubsystem&& _other) noexcept = delete;

        /**
         * @brief Renders an object based on the provided submission data.
         * @param _submission The render submission containing object details.
         */
        static void RenderObject(const RenderSubmission& _submission);

        /**
         * @brief Submits a camera for rendering.
         * @param _submission The camera submission containing view and projection data.
         */
        static void SubmitCamera(const CameraSubmission& _submission);

    private:
        /**
         * @brief Initializes the renderer subsystem.
         */
        void Initialize() override;

        /**
         * @brief Deinitializes the renderer subsystem.
         */
        void Deinitialize() override;

        CameraSubmission m_CameraSubmission; /**< Stores the current camera submission data. */

        friend class SceneSubsystem;
        friend class Engine;
    };
}
