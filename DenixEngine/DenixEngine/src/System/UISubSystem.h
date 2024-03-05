#pragma once

#include "SubSystem.h"

#include <SDL_video.h>

#include "Engine.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"


class UISubSystem: public SubSystem
{
public:
	UISubSystem();
	~UISubSystem() override;

	void Initialize() override;

	void Deinitialize() override;
};

