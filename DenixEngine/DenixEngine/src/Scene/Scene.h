#pragma once

#include "Core.h"

// Super Basic Scene class so I can start creating and testing features
// Will be intergrated into ECS later on
class Scene: public Object
{
public:
	// Constructors
	Scene() =  default;
	Scene(const ObjectInitializer& _object_init = ObjectInitializer::Get()): Object(_object_init) {}

	// Destructors
	~Scene() override = default;

	virtual void Draw() {}

	void RegisterObject(std::shared_ptr<GameObject> _object)
	{
		m_ObjectRegistry.push_back(std::move(_object));
	}

	void UnRegisterObject(const std::shared_ptr<GameObject>& _object)
	{
		if (const auto it = std::ranges::find(m_ObjectRegistry, _object); it != m_ObjectRegistry.end())
		{
			m_ObjectRegistry.erase(it);
		}
	}
protected:
	std::vector<std::shared_ptr<GameObject>> m_ObjectRegistry;
};