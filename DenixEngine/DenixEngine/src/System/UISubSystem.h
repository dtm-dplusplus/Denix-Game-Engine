#pragma once

#include "SubSystem.h"

class UISubSystem: public SubSystem
{
public:
	UISubSystem();
	~UISubSystem() override;

	void Initialize() override;

	void Deinitialize() override;
};

