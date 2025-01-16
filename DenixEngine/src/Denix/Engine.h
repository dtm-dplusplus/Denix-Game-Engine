#pragma once

#include "Denix/Core.h"
#include <vector>

int main(int argc, char** argv);

namespace Denix
{
    class SubsystemBase;
    class AudioSubsystem;
    class JobSubsystem;
    class Asset;
    class ReflectionSubsystem;
    class InputSubsystem;
    class EditorSubsystem;
    class RendererSubsystem;
    class UISubsystem;
    class PhysicsSubsystem;
    class SceneSubsystem;
    class AssetSubsystem;
    class FileSubsystem;
    class WindowSubsystem;
    class TimerSubsystem;
    class ProfileSubsystem;


    
    
    class Engine: public std::enable_shared_from_this<Engine>
    {
    public:
        Engine(std::string _projectName);
        virtual  ~Engine() = default;

        // Delete copy and move constructors and assignment operators
        Engine(const Engine& _other) = delete;
        Engine(Engine&& _other) noexcept = delete;
        Engine& operator=(const Engine& _other) = delete;
        Engine& operator=(Engine&& _other) noexcept = delete;

       
        void LoadConfig();
        void SaveConfig();
        Ref<Asset> GetStartupScene() const;
        static void SetStartupScene(const Ref<Asset>& _ref);

        static Ref<Engine> GetInstance() { return s_Engine; }

        std::string GetProjectName() const { return m_ProjectName; }

    protected:
        std::string m_ProjectName;
        Ref<Asset> m_StartupScene;
        std::string m_EngineConfigPath;

        virtual void PreInitialize();
        virtual void Initialize();
        virtual void Deinitialize();
        
    private:
        void EngineLoop();

        void Run();
        
        template<typename  T, typename ... Args>
        Ref<T> InitalizeSubsystem(Args&& ... _args)
        {
            try
            {
                // Check if T is derived from Actor
                Ref<T> subsystem = MakeRef<T>(std::forward<Args>(_args)...);
                static_assert(IsBase<SubsystemBase, T>(), "T must be derived from Subsystem");
                 subsystem->Initialize();
                return subsystem;
            }
            catch (const std::exception& e)
            {
                // Assert and terminate
                DE_LOG(LogEngine, Critical, "Failed to Initialize Subsystem: {0}", e.what())
                assert(false, e.what());
            }
            
            return nullptr;
        }


        /**
         * @brief Pointer to the engine instance
         */
        inline static Ref<Engine> s_Engine;

        size_t m_FrameCount;
        
        Ref<JobSubsystem> m_JobSubsystem;

        Ref<TimerSubsystem> m_TimerSubsystem;

        Ref<ReflectionSubsystem> m_ReflectionSubsystem;

        Ref<FileSubsystem> m_FileSubsystem;
        Ref<ProfileSubsystem> m_ProfileSubsystem;

        Ref<WindowSubsystem> m_WindowSubsystem;

        Ref<AssetSubsystem> m_AssetSubsystem;

        Ref<AudioSubsystem> m_AudioSubsystem;
        
        Ref<SceneSubsystem> m_SceneSubsystem;

        Ref<PhysicsSubsystem> m_PhysicsSubsystem;

        Ref<UISubsystem> m_UISubsystem;

        Ref<RendererSubsystem> m_RendererSubsystem;

        Ref<EditorSubsystem> m_EditorSubsystem;

        Ref<InputSubsystem> m_InputSubsystem;

        friend int ::main(int argc, char** argv);
    };

    // Defined in client
    Ref<Engine> MakeEngine();
}