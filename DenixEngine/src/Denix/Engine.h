/**
 * @file Engine.h
 * @brief Defines the core Engine class and its subsystems.
 */

#pragma once

#include "Denix/Core.h"
#include <vector>
#include "EngineConfig.h"

/**
 * @brief Entry point for the application.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Application exit code.
 */
int main(int argc, char** argv);

namespace Denix
{
    // Forward declarations for all subsystem classes
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

    /**
     * @class Engine
     * @brief Core engine class responsible for managing subsystems and game loop.
     */
    class Engine : public std::enable_shared_from_this<Engine>
    {
    public:
        /**
         * @brief Default constructor.
         */
        Engine() = default;

        /**
         * @brief Virtual destructor.
         */
        virtual ~Engine() = default;

        // Delete copy and move constructors and assignment operators
        Engine(const Engine& _other) = delete;
        Engine(Engine&& _other) noexcept = delete;
        Engine& operator=(const Engine& _other) = delete;
        Engine& operator=(Engine&& _other) noexcept = delete;

        /**
         * @brief Loads the engine configuration from a file.
         */
        void LoadConfig();

        /**
         * @brief Saves the current engine configuration to a file.
         */
        void SaveConfig();

        /**
         * @brief Gets the instance of the engine.
         * @return Shared reference to the engine instance.
         */
        static Ref<Engine> GetInstance() { return s_Engine; }

        /**
         * @brief Gets the engine configuration.
         * @return Reference to the engine configuration object.
         */
        static EngineConfig& GetConfig() { return s_Engine->m_Config; }

    protected:
        /**
         * @brief Pre-initialization logic for the engine.
         */
        virtual void PreInitialize();

        /**
         * @brief Initializes the engine and its subsystems.
         */
        virtual void Initialize();

        /**
         * @brief Deinitializes the engine and cleans up resources.
         */
        virtual void Deinitialize();

    private:
        /**
         * @brief Main engine loop.
         */
        void EngineLoop();

        /**
         * @brief Starts the engine execution.
         */
        void Run();

        /**
         * @brief Initializes a subsystem of type T.
         * @tparam T Type of subsystem to initialize.
         * @tparam Args Parameter pack for subsystem construction.
         * @param _args Arguments to pass to the subsystem constructor.
         * @return Shared reference to the initialized subsystem.
         */
        template <typename T, typename... Args>
        Ref<T> InitalizeSubsystem(Args&&... _args);

        /**
         * @brief Pointer to the singleton instance of the engine.
         */
        inline static Ref<Engine> s_Engine;

        /**
         * @brief Engine configuration settings.
         */
        EngineConfig m_Config;

        /**
         * @brief Number of frames rendered since engine start.
         */
        size_t m_FrameCount;

        // Engine subsystems
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

        /**
         * @brief Grants main() function access to private members.
         */
        friend int ::main(int argc, char** argv);
    };

    /**
     * @brief Factory function to create an engine instance.
     * @return Shared reference to a new engine instance.
     */
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
