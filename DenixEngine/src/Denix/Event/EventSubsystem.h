#pragma once
#include "Denix/Core/Subsystem.h"
#include <SDL3/SDL_events.h>

namespace Denix
{
    class SDL_GLWindow;
    class InputSubsystem;
    class WindowSubsystem;

    class EventSubsystem : public Subsystem<EventSubsystem>
    {
    public:
        EventSubsystem() = default;
        ~EventSubsystem() override = default;

        EventSubsystem(const EventSubsystem& _other) = delete;
        EventSubsystem(EventSubsystem&& _other) noexcept = delete;
        EventSubsystem& operator=(const EventSubsystem& _other) = delete;
        EventSubsystem& operator=(EventSubsystem&& _other) noexcept = delete;

    private:
        void ProcessApplicationEvent(const SDL_Event& _event);
        void ProcessDisplayEvent(const SDL_Event& _event);
        void ProcessWindowEvent(const SDL_Event& _event);
        void ProcessFileEvent(const SDL_Event& _event);
        void ProcessAudioEvent(const SDL_Event& _event);
        void ProcessRenderEvent(const SDL_Event& _event);

        void Initialize() override;
        void Deinitialize() override;
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        //SDL_Event m_Event;
        WRef<SDL_GLWindow> m_WindowRef;
        WRef<InputSubsystem> m_InputRef;
        WRef<WindowSubsystem> m_WindowSubsystemRef;

        friend class Engine;
    };
}
