#pragma once

#include "Denix/Core.h"
#include <vector>

#include "EngineConfig.h"

int main(int argc, char** argv);

namespace Denix
{
    class SubsystemBase;
    class AudioSubsystem;
    class JobSubsystem;
    class Asset;
    class ReflectionSubsystem;
    class EventSubsystem;
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


    class Engine : public std::enable_shared_from_this<Engine>
    {
    public:
        Engine() = default;
        virtual ~Engine() = default;

        // Delete copy and move constructors and assignment operators
        Engine(const Engine& _other) = delete;
        Engine(Engine&& _other) noexcept = delete;
        Engine& operator=(const Engine& _other) = delete;
        Engine& operator=(Engine&& _other) noexcept = delete;


        void LoadConfig();
        void SaveConfig();

        static Ref<Engine> GetInstance() { return s_Engine; }

        static EngineConfig& GetConfig() { return s_Engine->m_Config; }

    protected:
        virtual void PreInitialize();
        virtual void Initialize();
        virtual void Deinitialize();

    private:
        void EngineLoop();

        void Run();

        template <typename T, typename... Args>
        Ref<T> InitalizeSubsystem(Args&&... _args);


        /**
         * @brief Pointer to the engine instance
         */
        inline static Ref<Engine> s_Engine;

        EngineConfig m_Config;

        size_t m_FrameCount;

        Ref<JobSubsystem> m_JobSubsystem;

        Ref<TimerSubsystem> m_TimerSubsystem;

        Ref<ReflectionSubsystem> m_ReflectionSubsystem;

        Ref<FileSubsystem> m_FileSubsystem;

        Ref<InputSubsystem> m_InputSubsystem;

        Ref<EventSubsystem> m_EventSubsystem;

        Ref<ProfileSubsystem> m_ProfileSubsystem;

        Ref<WindowSubsystem> m_WindowSubsystem;

        Ref<AssetSubsystem> m_AssetSubsystem;

        Ref<AudioSubsystem> m_AudioSubsystem;

        Ref<SceneSubsystem> m_SceneSubsystem;

        Ref<PhysicsSubsystem> m_PhysicsSubsystem;

        Ref<UISubsystem> m_UISubsystem;

        Ref<RendererSubsystem> m_RendererSubsystem;

        Ref<EditorSubsystem> m_EditorSubsystem;


        friend int ::main(int argc, char** argv);
    };

    // Defined in client
    Ref<Engine> MakeEngine();

    template <typename T, typename... Args>
    Ref<T> Engine::InitalizeSubsystem(Args&&... _args)
    {
        static_assert(IsBase<SubsystemBase, T>(), "Class must be derived from Subsystem");

        // Check if T is derived from Actor
        if (Ref<T> subsystem = MakeRef<T>(std::forward<Args>(_args)...))
        {
            subsystem->Initialize();
            return subsystem;
        }

        DE_LOG(LogEngine, Error, "Failed to initialize subsystem: {0}", typeid(T).name());

        return nullptr;
    }
}
