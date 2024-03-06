#pragma once
#include "SubSystem.h"

#include "../Core.h"

/**
 * \brief 
 */
class ShaderSubSystem: public SubSystem
{
public:
	ShaderSubSystem()
	{
		s_ShaderSubSystem = this;
	}

	~ShaderSubSystem() override
	{
		s_ShaderSubSystem = nullptr;
	}

	static ShaderSubSystem* Get() { return s_ShaderSubSystem; }

	void Initialize() override
	{
		DE_LOG(Log, Trace, "Shader SubSystem Initialized")
		m_Initialized = true;
	}

	void Deinitialize() override
	{
		DE_LOG(Log, Trace, "Shader SubSystem Initialized")
		m_Initialized = false;
	}

	bool LoadShader(const std::string& _filepath)
	{
		
	}

	
private:
	static ShaderSubSystem* s_ShaderSubSystem;
};

