#pragma once

#include "Denix/Core.h"
#include <vector>

int main(int argc, char** argv);

namespace Denix
{
    class Subsystem;
    class ReflectionSubsystem;
    class InputSubsystem;
    class EditorSubsystem;
    class RendererSubsystem;
    class UISubsystem;
    class PhysicsSubsystem;
    class SceneSubsystem;
    class ResourceSubsystem;
    class FileSubsystem;
    class WindowSubsystem;
    class TimerSubsystem;

    class Engine
    {
    public:
        Engine();
        virtual  ~Engine();

        // Delete copy and move constructors and assignment operators
        Engine(const Engine& _other) = delete;
        Engine(Engine&& _other) noexcept = delete;
        Engine& operator=(const Engine& _other) = delete;
        Engine& operator=(Engine&& _other) noexcept = delete;
        
        virtual void Initialize();
        virtual void Deinitialize();

        void Run();
        void LoadConfig();
        void SaveConfig();

        static Engine& Get() { return *s_Engine; }

        std::string GetProjectName() const { return m_ProjectName; }

    protected:
        std::string m_ProjectName;

    private:
        static Engine* s_Engine;

        template<typename  T, typename ... Args>
        URef<T> InitalizeSubsystem(Args&& ... _args)
        {
            URef<T> subsystem = MakeURef<T>(std::forward<Args>(_args)...); 

            try
            {
                 subsystem->Initialize();
            }
            catch (const std::exception& e)
            {
                // Assert and terminate
                DE_LOG(LogEngine, Critical, "Failed to Initialize Subsystem: {0}", e.what())
                assert(false, e.what());
            }
            
            m_Subsystems.push_back(subsystem.get());
            return subsystem;
        }

    // Usufeul vector for deinitializing subsystems in reverse order
    std::vector<Subsystem*> m_Subsystems;

    URef<TimerSubsystem> m_TimerSubsystem;

    URef<ReflectionSubsystem> m_ReflectionSubsystem;

    URef<WindowSubsystem> m_WindowSubsystem;

    URef<FileSubsystem> m_FileSubsystem;

    URef<ResourceSubsystem> m_ResourceSubsystem;

    URef<SceneSubsystem> m_SceneSubsystem;

    URef<PhysicsSubsystem> m_PhysicsSubsystem;

    URef<UISubsystem> m_UISubsystem;

    URef<RendererSubsystem> m_RendererSubsystem;

    URef<EditorSubsystem> m_EditorSubsystem;

    URef<InputSubsystem> m_InputSubsystem;

    friend int ::main(int argc, char** argv);
    };

    // Defined in client
    URef<Engine> CreateEngine();
}
