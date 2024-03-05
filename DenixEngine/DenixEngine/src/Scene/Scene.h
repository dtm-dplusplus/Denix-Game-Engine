#pragma once

#include "Object.h"
#include "GameObject.h"
#include "Component.h"
#include "Camera.h"


// Basic Scene class so I can start creating and testing features
class Scene: public Object
{
public:
	Scene(const ObjectInitializer& _object_init) : Object(_object_init) {}
	~Scene() override {}

	virtual bool Load() = 0;

	virtual void Unload() 
	{
		m_SceneObjects.clear();
	}

	void Update(float _deltaTime) override {}

	Ref<Camera> GetCamera() { return m_Camera; }
protected:
	
	std::vector<Ref<GameObject>> m_SceneObjects;

	Ref<Camera> m_Camera;

	friend class SceneSubSystem;
};



