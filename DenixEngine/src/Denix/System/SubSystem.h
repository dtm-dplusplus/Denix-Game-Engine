#pragma once

namespace Denix
{
	class Subsystem
	{
	public:
		Subsystem() = default;
		virtual ~Subsystem() = default;
	

		virtual void Initialize() {}
		virtual void Deinitialize() {}
		bool IsInitialized() const { return m_Initialized; }

		virtual void Update(float _deltaTime){}
		virtual void PreUpdate(float _deltaTime) {}
		virtual void PostUpdate(float _deltaTime) {}

		bool IsEnabled() const { return m_Enabled; }
		bool& IsEnabled() { return m_Enabled; }
		void SetEnabled(const bool _enabled) { m_Enabled = _enabled; }

	protected:
		bool m_Initialized = false;
		bool m_Enabled = true;

		friend class Engine;
	};
}