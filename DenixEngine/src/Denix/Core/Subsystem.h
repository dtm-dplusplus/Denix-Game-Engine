#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Thread/Counter.h"

namespace Denix
{
    class SubsystemBase
    {
    public:
        SubsystemBase() = default;
        virtual ~SubsystemBase() = default;

        SubsystemBase(const SubsystemBase& _other) = delete;
        SubsystemBase(SubsystemBase&& _other) noexcept = delete;
        SubsystemBase& operator=(const SubsystemBase& _other) = delete;
        SubsystemBase& operator=(SubsystemBase&& _other) noexcept = delete;

        virtual void Initialize() = 0;
        virtual void Deinitialize() = 0;

        virtual void Update(float _deltaTime, const Ref<Counter>& _waitCounter)
        {
        }

        virtual void PreUpdate(float _deltaTime, const Ref<Counter>& _waitCounter)
        {
        }

        virtual void PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter)
        {
        }

        virtual bool IsEnabled() const { return m_Enabled; }
        virtual bool& IsEnabled() { return m_Enabled; }
        virtual void SetEnabled(const bool _enabled) { m_Enabled = _enabled; }

    protected:
        bool m_Enabled = true;

        friend class Engine;
    };

    template <class T>
    class Subsystem : public SubsystemBase, public std::enable_shared_from_this<T>
    {
    public:
        Subsystem() = default;
        ~Subsystem() override = default;

        Subsystem(const Subsystem& _other) = delete;
        Subsystem(Subsystem&& _other) noexcept = delete;
        Subsystem& operator=(const Subsystem& _other) = delete;
        Subsystem& operator=(Subsystem&& _other) noexcept = delete;

        void Initialize() override
        {
            s_Instance = this->shared_from_this();
        }

        void Deinitialize() override
        {
            s_Instance.reset();
        }

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override
        {
        }

        void PreUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) override
        {
        }

        void PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) override
        {
        }

        bool IsEnabled() const override { return m_Enabled; }
        bool& IsEnabled() override { return m_Enabled; }
        void SetEnabled(const bool _enabled) override { m_Enabled = _enabled; }

        static Ref<T> GetInstance() { return s_Instance; }

    protected:
        bool m_Enabled = true;

        inline static Ref<T> s_Instance;

        friend class Engine;
    };
}
