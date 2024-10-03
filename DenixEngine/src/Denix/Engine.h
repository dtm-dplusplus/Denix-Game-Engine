#pragma once

#include "Denix/Core.h"
#include <unordered_map>
#include <vector>

#include "Denix/System/SubSystem.h"

int main(int argc, char** argv);

namespace Denix
{
    class Engine
    {
    public:

        Engine();
        virtual  ~Engine();

        void Initialize();
        void Deinitialize();

        void Run();
        void LoadConfig();
        void SaveConfig();

        static Engine& Get() { return *s_Engine; }

        std::string GetProjectName() const { return m_ProjectName; }

    public:
        virtual void PreInitialize();
        virtual void PostInitialize();

    protected:
        std::string m_ProjectName;


    private:
        static Engine* s_Engine;

        std::vector<Subsystem*> m_Subsystems;

        class TimerSubsystem* m_TimerSubSystem;

        class WindowSubsystem* m_WindowSubsystem;

        class FileSubsystem* m_FileSubSystem;

        class ResourceSubsystem* m_ResourceSubSystem;

        class SceneSubsystem* m_SceneSubSystem;

        class PhysicsSubsystem* m_PhysicsSubSystem;

        class UISubsystem* m_UISubsystem;

        class RendererSubsystem* m_RendererSubSystem;

        class EditorSubsystem* m_EditorSubSystem;

        class InputSubsystem* m_InputSubsystem;

        friend int ::main(int argc, char** argv);
    };

    // Defined in client
    Engine* CreateEngine();
}
