#pragma once
#include "../Core.h"

class SubSystem
{
public:
	SubSystem() = default;
	virtual ~SubSystem() = default;
	

	virtual void Initialize() {}
	virtual void Deinitialize() {}

	virtual void Update(float _deltaTime){}
	bool IsInitialized() const { return m_Initialized; }


protected:
	bool m_Initialized = false;

	friend class Engine;
};

