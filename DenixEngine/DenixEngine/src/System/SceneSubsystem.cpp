#include "DePch.h"
#include "SceneSubsystem.h"
#include "WindowSubsystem.h"

SceneSubSystem* SceneSubSystem::s_SceneSubSystem{ nullptr };


void SceneSubSystem::Update(float _deltaTime)
{
	const WindowSubSystem* windowSystem = WindowSubSystem::Get();

	// Update Camera
	if (const Ref<Camera> cam = m_ActiveScene->GetCamera())
	{
		cam->Aspect = windowSystem->GetWindow()->GetWindowSize();
		cam->Update(_deltaTime);

		// Camera Movement
		if (const Uint8* keyboard = SDL_GetKeyboardState(NULL))
		{
			if (keyboard[SDL_SCANCODE_W])
			{
				DE_LOG(LogScene, Info, "W Pressed")
					cam->GetTransformComponent()->GetPosition().z += cam->MoveSpeed * _deltaTime;
			}
			if (keyboard[SDL_SCANCODE_S])
			{
				DE_LOG(LogScene, Info, "S Pressed")
					cam->GetTransformComponent()->GetPosition().z -= cam->MoveSpeed * _deltaTime;
			}
			if (keyboard[SDL_SCANCODE_A])
			{
				DE_LOG(LogScene, Info, "A Pressed")
					cam->GetTransformComponent()->GetPosition().x += cam->MoveSpeed * _deltaTime;

			}
			if (keyboard[SDL_SCANCODE_D])
			{
				DE_LOG(LogScene, Info, "D Pressed")
					cam->GetTransformComponent()->GetPosition().x -= cam->MoveSpeed * _deltaTime;
			}
			if (keyboard[SDL_SCANCODE_E])
			{
				DE_LOG(LogScene, Info, "E Pressed")
					cam->GetTransformComponent()->GetPosition().y -= cam->MoveSpeed * _deltaTime;
			}
			if (keyboard[SDL_SCANCODE_Q])
			{
				DE_LOG(LogScene, Info, "Q Pressed")
					cam->GetTransformComponent()->GetPosition().y += cam->MoveSpeed * _deltaTime;
			}
		}
	}

	// Scene update implementation 
	m_ActiveScene->Update(_deltaTime);

	// Update the scene GameObjects
	for(const auto& gameObject : m_ActiveScene->m_SceneObjects)
	{
		const Ref<TransformComponent> transform = gameObject->GetTransformComponent();
		const Ref<PhysicsComponent> physics = gameObject->GetPhysicsComponent();

		if(transform && physics)
		{
			// Transform Component Update
			transform->Update(_deltaTime);

			// Physics Component Update
			if (m_ActiveScene->m_IsLive)
			{
				if (physics->m_IsSimulated)
				{
					// Temporarily store the position
					physics->m_TempPosition = transform->GetPosition();

					physics->Update(_deltaTime);

					// Update the position
					transform->SetPosition(physics->m_TempPosition);
				}
			}
		}
		
		// GameObject Update implementation
		gameObject->Update(_deltaTime);
	}

	// Update the Scene Panel
	ScenePanel(_deltaTime);
}