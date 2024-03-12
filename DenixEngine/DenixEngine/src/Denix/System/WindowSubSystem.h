#pragma once

#include "Denix/Core.h"
#include "SubSystem.h"
#include "Denix/Video/Window.h"


namespace Denix
{
	class WindowSubSystem: public SubSystem
	{
	public:
		WindowSubSystem()
		{
			s_WindowSubSystem = this;
		}

		~WindowSubSystem() override
		{
			s_WindowSubSystem = nullptr;
		}

		static WindowSubSystem* Get() { return s_WindowSubSystem; }

		void Initialize() override;

		void Deinitialize() override;

		Ref<SDL_GLWindow> GetWindow() const { return m_Window; }

	private:
		Ref<SDL_GLWindow> m_Window;

		static WindowSubSystem* s_WindowSubSystem;


		friend class Engine;
	};
}
