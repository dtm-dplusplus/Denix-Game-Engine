#pragma once

class SubSystem
{
public:
	SubSystem() {}
	virtual ~SubSystem() {}

	virtual void Initialize() {}
	virtual void Deinitialize() {}

	bool IsInitialized() const { return m_Initialized; }

protected:
	bool m_Initialized;

	friend class Engine;
};

