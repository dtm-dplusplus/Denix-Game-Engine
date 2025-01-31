/**
 * @file UISubsystem.h
 * @brief Declaration of the UISubsystem class, responsible for managing UI components.
 */

#pragma once

#include <ft2build.h>
#include <glm/mat4x4.hpp>
#include FT_FREETYPE_H

#include "Denix/Core/Subsystem.h"
#include "Denix/UI/Widget/Canvas.h"

namespace Denix
{
    /**
     * @class UISubsystem
     * @brief Manages UI rendering and widget handling.
     */
    class UISubsystem : public Subsystem<UISubsystem>
    {
    public:
        /**
         * @brief Default constructor.
         */
        UISubsystem() = default;

        /**
         * @brief Default destructor.
         */
        ~UISubsystem() override = default;

        UISubsystem(const UISubsystem& _other) = delete;
        UISubsystem(UISubsystem&& _other) noexcept = delete;
        UISubsystem& operator=(const UISubsystem& _other) = delete;
        UISubsystem& operator=(UISubsystem&& _other) noexcept = delete;

        /**
         * @brief Stores FreeType library instance.
         */
        inline static FT_Library m_FtLibrary;

        /**
         * @brief Stores FreeType font face instance.
         */
        inline static FT_Face m_Face;

        std::vector<Ref<Canvas>> m_Widgets; /**< Stores UI widgets. */
        glm::mat4 m_Projection; /**< UI projection matrix. */
        glm::mat4 m_View; /**< UI view matrix. */

    private:
        /**
         * @brief Handles rendering of UI components.
         */
        void RenderUISubmission();

        /**
         * @brief Initializes the UI subsystem.
         */
        void Initialize() override;

        /**
         * @brief Deinitializes the UI subsystem.
         */
        void Deinitialize() override;

        /**
         * @brief Updates UI elements each frame.
         * @param _deltaTime The time step.
         * @param _waitCounter Synchronization counter.
         */
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        friend class Engine;
        friend class EditorSubsystem;
        friend class WindowSubsystem;
    };
}
