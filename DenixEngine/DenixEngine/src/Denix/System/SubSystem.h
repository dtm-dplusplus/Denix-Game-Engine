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

		virtual void Update(float _deltaTime){}
		virtual void LateUpdate(float _deltaTime) {}
		bool IsInitialized() const { return m_Initialized; }

		bool IsActive() const { return m_Active; }
		bool& IsActive() { return m_Active; }

	protected:
		bool m_Initialized = false;
		bool m_Active = true;

		friend class Engine;
	};
}