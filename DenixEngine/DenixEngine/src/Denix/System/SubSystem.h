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

	protected:
		bool m_Initialized = false;

		friend class Engine;
	};
}