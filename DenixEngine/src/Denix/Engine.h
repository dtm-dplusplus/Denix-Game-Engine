#pragma once

#include "Denix/Core.h"
#include <vector>

int main(int argc, char** argv);

namespace Denix
{
    class JobSubsystem;
    class Asset;
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
    class ProfileSubsystem;

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
        void ParallelLoop();
        void SequentialLoop();

        void Run();
        void LoadConfig();
        void SaveConfig();
        Ref<Asset> GetStartupScene() const;
        static void SetStartupScene(const Ref<Asset>& _ref);

        static Engine* Get() { return s_Engine; }

        std::string GetProjectName() const { return m_ProjectName; }


        Ref<bool> m_ParallelLoop = MakeRef<bool>(true);
        bool m_DummySubsystemA = true;
        bool m_DummySubsystemB = true;
    protected:
        std::string m_ProjectName;
        Ref<Asset> m_StartupScene;
        std::string m_EngineConfigPath;
    private:
        static Engine* s_Engine;

        template<typename  T, typename ... Args>
        Ref<T> InitalizeSubsystem(Args&& ... _args)
        {
            Ref<T> subsystem = MakeRef<T>(std::forward<Args>(_args)...);

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

        // Useful vector for deinitializing subsystems in reverse order
        std::vector<Subsystem*> m_Subsystems;

        Ref<JobSubsystem> m_JobSubsystem;

        Ref<TimerSubsystem> m_TimerSubsystem;

        Ref<ReflectionSubsystem> m_ReflectionSubsystem;

        Ref<FileSubsystem> m_FileSubsystem;
        Ref<ProfileSubsystem> m_ProfileSubsystem;

        Ref<WindowSubsystem> m_WindowSubsystem;

        Ref<ResourceSubsystem> m_ResourceSubsystem;

        Ref<SceneSubsystem> m_SceneSubsystem;

        Ref<PhysicsSubsystem> m_PhysicsSubsystem;

        Ref<UISubsystem> m_UISubsystem;

        Ref<RendererSubsystem> m_RendererSubsystem;

        Ref<EditorSubsystem> m_EditorSubsystem;

        Ref<InputSubsystem> m_InputSubsystem;

        friend int ::main(int argc, char** argv);
    };

    // Defined in client
   URef<Engine> CreateEngine();
}