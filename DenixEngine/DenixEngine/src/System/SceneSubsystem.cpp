#include "DePch.h"
#include "SceneSubsystem.h"
#include "../Engine.h"

void SceneSubSystem::Update()
{
	const Engine &engine = Engine::Get();
	const glm::vec2 winSize = engine.GetEngineWindow()->GetWindowSize();

	if (m_ActiveScene)
	{
		// Update the Camera
		m_ActiveScene->m_Camera->Aspect.x = winSize.x;
		m_ActiveScene->m_Camera->Aspect.y = winSize.y;
		m_ActiveScene->m_Camera->Update();

		// Update the scene
		m_ActiveScene->Update();

		// Update the scene GameObjects
		for(const auto& gameObject : m_ActiveScene->m_SceneObjects)
		{
			// Transform Component
			gameObject->GetTransformComponent()->Update();

			// Physics Component

			// Custom Update Functions
			gameObject->Update();
		}
	}
}
