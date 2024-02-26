#pragma once
#include <SDL_video.h>

#include "SubSystem.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "Engine.h"

class UISubSystem: public SubSystem
{
public:
	UISubSystem() {}
	~UISubSystem() override {}

	void Initialize() override 
	{
		
	}

	void Deinitialize() override {}
};

