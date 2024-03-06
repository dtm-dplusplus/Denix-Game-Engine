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
	~Scene() override = default;

	virtual bool Load() = 0;

	virtual void Unload() 
	{
		m_SceneObjects.clear();
		m_Camera.reset();
	}

	void BeginScene() override {}
	void EndScene() override {}

	void Update(float _deltaTime) override{}

	bool IsLive() const { return m_IsLive; }

	glm::vec3 GetGravity() const { return m_SceneGravity; }

	Ref<Camera> GetCamera() { return m_Camera; }
protected:
	/** determine if the engine is in editor or tool side mode.
	 * True if the scene is being played. False if in editor mode.
	 */
	bool m_IsLive = false;

	glm::vec3 m_SceneGravity = glm::vec3(0.0f, -9.8f, 0.0f);

	/** List of Objects in the scene */
	std::vector<Ref<GameObject>> m_SceneObjects;

	Ref<Camera> m_Camera;

	friend class SceneSubSystem;
};



