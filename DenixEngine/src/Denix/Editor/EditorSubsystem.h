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

    class EditorSubsystem : public Subsystem<EditorSubsystem>
    {
    public:
        EditorSubsystem() = default;

        ~EditorSubsystem() override = default;

        EditorSubsystem(const EditorSubsystem& _other) = delete;
        EditorSubsystem(EditorSubsystem&& _other) noexcept = delete;
        EditorSubsystem& operator=(const EditorSubsystem& _other) = delete;
        EditorSubsystem& operator=(EditorSubsystem&& _other) noexcept = delete;

        void SetActiveScene(const Ref<Scene>& _scene);

        static ImGuiID GetDockLeftID() { return s_Instance->DockLeftID; }
        static ImGuiID GetDockRightID() { return s_Instance->DockRightID; }
        static ImGuiID GetDockDownID() { return s_Instance->DockDownID; }

        template <typename T, typename... Args>
        static Ref<T> AddEditorWidget(Args&&... _args)
        {
            static_assert(IsBase<EditorWidget, T>(), "Class must be derived from EditorWidget");

            // Check if T is derived from Actor
            if (Ref<T> widget = MakeRef<T>(std::forward<Args>(_args)...))
            {
                s_Instance->m_EditorWidgets.emplace_back(widget);
                return CastRef<T>(s_Instance->m_EditorWidgets.back());
            }

            DE_LOG(LogEditor, Error, "Failed to add Editor Widget: {0}", typeid(T).name());
            return nullptr;
        }

        static void RemoveEditorWidget(const Ref<EditorWidget>& _widget)
        {
            if (!_widget) return;

            std::erase(s_Instance->m_EditorWidgets, _widget);
        }

    private:
        void MainMenuBar();

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
        void Initialize() override;
        void Deinitialize() override;

        static void NewFrame();
        static void RenderUI();
        static void PresentFrame();
        void ViewportUpdate();


        ImGuiID DockLeftID;
        ImGuiID DockRightID;
        ImGuiID DockDownID;

        WRef<SDL_GLWindow> m_WindowRef;

        std::vector<Ref<EditorWidget>> m_EditorWidgets;
        bool ShowDemoWindow = false;
        bool ShowPlotDemoWindow = false;

        float m_DragSpeed;

        WRef<Scene> m_ActiveScene;

        Ref<SceneOrganizerWidget> m_SceneOrganizerWidget;

        Ref<ActorDetailsWidget> m_ActorDetailsWidget;

        Ref<AssetBrowserWidget> m_AssetBrowserWidget;

        Ref<PerformanceSettingsWidget> m_PerformanceSettingsWidget;

        Ref<EngineProfilerWidget> m_EngineProfilerWidget;

        Ref<InputDebuggerWidget> m_InputDebuggerWidget;

        friend class Engine;
        friend class EditorSubsystem;
        friend class WindowSubsystem;
    };
}
