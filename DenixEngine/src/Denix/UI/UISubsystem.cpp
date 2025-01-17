#include "UISubsystem.h"

#include "backends/imgui_impl_sdl3.h"
#include "Denix/Core/File/FileSubsystem.h"

#include "Denix/Video/Window/Window.h"
#include "Denix/Video/WindowSubsystem.h"

namespace Denix
{
	void UISubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogUI, Warn, "UI Subsystem Initializing")
		FT_Error error;

		error = FT_Init_FreeType(&m_FtLibrary); /* initialize library */
		if (error)
		{
			DE_LOG(LogRender, Error, "Failed to initialize FreeType Library")
		}
		
		DE_LOG(LogUI, Info, "UI Subsystem Initialized")
	}

	void UISubsystem::Deinitialize()
	{
		DE_LOG(LogUI, Trace, "UI Subsystem Deinitializing")
		
		FT_Done_FreeType(m_FtLibrary);
		Subsystem::Deinitialize();
		DE_LOG(LogUI, Trace, "UI Subsystem Deinitialized")
	}
	void UISubsystem::Update(float _deltaTime)
	{
		Subsystem::Update(_deltaTime);
	}
}
