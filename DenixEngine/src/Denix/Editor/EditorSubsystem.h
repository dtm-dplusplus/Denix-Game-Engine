/**
 * @file EditorSubsystem.h
 * @brief Declaration of the EditorSubsystem class, responsible for managing the editor interface and widgets.
 */

#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Subsystem.h"
#include "Denix/Editor/EditorWidget.h"
#include "imgui.h"
#include "implot.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Denix
{
    class SDL_GLWindow;
    class Scene;
    class SceneOrganizerWidget;
    class ActorDetailsWidget;
    class AssetBrowserWidget;
    class InputDebuggerWidget;
    class EngineProfilerWidget;
    class PerformanceSettingsWidget;

    /**
     * @class EditorSubsystem
     * @brief Manages the editor interface and its widgets.
     */
    class EditorSubsystem : public Subsystem<EditorSubsystem>
    {
    public:
        /**
         * @brief Default constructor.
         */
        EditorSubsystem() = default;

        /**
         * @brief Default destructor.
         */
        ~EditorSubsystem() override = default;

        EditorSubsystem(const EditorSubsystem& _other) = delete;
        EditorSubsystem(EditorSubsystem&& _other) noexcept = delete;
        EditorSubsystem& operator=(const EditorSubsystem& _other) = delete;
        EditorSubsystem& operator=(EditorSubsystem&& _other) noexcept = delete;

        /**
         * @brief Sets the active scene in the editor.
         * @param _scene The active scene reference.
         */
        void SetActiveScene(const Ref<Scene>& _scene);

        /**
         * @brief Retrieves the dock ID for the left panel.
         * @return ImGuiID of the left dock panel.
         */
        static ImGuiID GetDockLeftID() { return s_Instance->DockLeftID; }

        /**
         * @brief Retrieves the dock ID for the right panel.
         * @return ImGuiID of the right dock panel.
         */
        static ImGuiID GetDockRightID() { return s_Instance->DockRightID; }

        /**
         * @brief Retrieves the dock ID for the bottom panel.
         * @return ImGuiID of the bottom dock panel.
         */
        static ImGuiID GetDockDownID() { return s_Instance->DockDownID; }

        /**
         * @brief Adds an editor widget.
         * @tparam T The type of editor widget to add.
         * @tparam Args Variadic template arguments for widget construction.
         * @param _args Arguments forwarded to the widget constructor.
         * @return A reference to the newly added widget.
         */
        template <typename T, typename... Args>
        static Ref<T> AddEditorWidget(Args&&... _args)
        {
            static_assert(IsBase<EditorWidget, T>(), "Class must be derived from EditorWidget");

            if (Ref<T> widget = MakeRef<T>(std::forward<Args>(_args)...))
            {
                s_Instance->m_EditorWidgets.emplace_back(widget);
                return CastRef<T>(s_Instance->m_EditorWidgets.back());
            }

            DE_LOG(LogEditor, Error, "Failed to add Editor Widget: {0}", typeid(T).name());
            return nullptr;
        }

        /**
         * @brief Removes an editor widget.
         * @param _widget Reference to the widget to remove.
         */
        static void RemoveEditorWidget(const Ref<EditorWidget>& _widget)
        {
            if (!_widget) return;

            std::erase(s_Instance->m_EditorWidgets, _widget);
        }

    private:
        /**
         * @brief Handles the main menu bar rendering.
         */
        void MainMenuBar();

        /**
         * @brief Updates the editor subsystem.
         * @param _deltaTime Time since last update.
         * @param _waitCounter Reference to a wait counter.
         */
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        /**
         * @brief Initializes the editor subsystem.
         */
        void Initialize() override;

        /**
         * @brief Deinitializes the editor subsystem.
         */
        void Deinitialize() override;

        /**
         * @brief Starts a new frame.
         */
        static void NewFrame();

        /**
         * @brief Renders the UI.
         */
        static void RenderUI();

        /**
         * @brief Presents the frame.
         */
        static void PresentFrame();

        /**
         * @brief Updates the viewport.
         */
        void ViewportUpdate();

        ImGuiID DockLeftID; /**< Dock ID for the left panel. */
        ImGuiID DockRightID; /**< Dock ID for the right panel. */
        ImGuiID DockDownID; /**< Dock ID for the bottom panel. */

        WRef<SDL_GLWindow> m_WindowRef; /**< Weak reference to the SDL window. */
        std::vector<Ref<EditorWidget>> m_EditorWidgets; /**< List of active editor widgets. */
        bool ShowDemoWindow = false; /**< Flag to show the ImGui demo window. */
        bool ShowPlotDemoWindow = false; /**< Flag to show the ImPlot demo window. */
        float m_DragSpeed; /**< Drag speed setting. */
        WRef<Scene> m_ActiveScene; /**< Weak reference to the active scene. */

        Ref<SceneOrganizerWidget> m_SceneOrganizerWidget; /**< Scene organizer widget reference. */
        Ref<ActorDetailsWidget> m_ActorDetailsWidget; /**< Actor details widget reference. */
        Ref<AssetBrowserWidget> m_AssetBrowserWidget; /**< Asset browser widget reference. */
        Ref<PerformanceSettingsWidget> m_PerformanceSettingsWidget; /**< Performance settings widget reference. */
        Ref<EngineProfilerWidget> m_EngineProfilerWidget; /**< Engine profiler widget reference. */
        Ref<InputDebuggerWidget> m_InputDebuggerWidget; /**< Input debugger widget reference. */

        friend class Engine;
        friend class EditorSubsystem;
        friend class WindowSubsystem;
    };
}
