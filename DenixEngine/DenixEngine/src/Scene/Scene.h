#pragma once

#include "Object/Object.h"
#include "Object/GameObject.h"

// Basic Scene class so I can start creating and testing features
class Scene: public Object
{
public:
	Scene(const ObjectInitializer& _object_init) : Object(_object_init) {}
	~Scene() override {}

	virtual bool Load() { return true; }
	virtual void Unload() 
	{
		m_SceneObjects.clear();
	}

	void Update() override {}
	virtual void Draw() {}

protected:
	
	std::vector<Ref<GameObject>> m_SceneObjects;

	friend class SceneSubSystem;
};



