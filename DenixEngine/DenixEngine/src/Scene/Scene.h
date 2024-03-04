#pragma once

#include "Object/Object.h"
#include "Object/GameObject.h"

struct Camera
{
	// Camera
	bool IsPerspective = true;
	float Fov = 45.f;
	float NearPlane = 0.1f;
	float FarPlane = 100.f;

	glm::vec3 CamPos = { 0.f, 0.f, -2.5f };
};
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

	void Update() override
	{
		for (const auto& obj : m_SceneObjects)
		{
			obj->Update();
		}

		// Update Physics Components

		// Update Transform Components

	}

protected:
	
	std::vector<Ref<GameObject>> m_SceneObjects;

	friend class SceneSubSystem;
};



