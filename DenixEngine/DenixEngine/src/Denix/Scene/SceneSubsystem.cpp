#include "depch.h"
#include "SceneSubsystem.h"
#include "Denix/Video/Window/WindowSubsystem.h"
#include "Denix/Video/Renderer/RendererSubsystem.h"
#include "Denix/Physics/PhysicsSubSystem.h"
#include "Denix/Editor/EditorSubsystem.h"

#include "Denix/Engine.h"
#include "imgui.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Physics/Collider.h"
#include "Denix/Resource/ResourceSubsystem.h"

namespace Denix
{
	SceneSubsystem* SceneSubsystem::s_SceneSubsystem{ nullptr };

	void SceneSubsystem::Initialize()
	{
		s_InputSubsystem = InputSubsystem::Get();
		s_WindowSubsystem = WindowSubsystem::Get();
		s_RendererSubsystem = RendererSubsystem::Get();
		s_PhysicsSubsystem = PhysicsSubsystem::Get();
		DE_LOG(LogSceneSubSystem, Trace, "Scene Subsystem Initialized")

		m_Initialized = true;
	}

	void SceneSubsystem::Deinitialize()
	{
		m_LoadedScenes.clear();

		DE_LOG(LogSceneSubSystem, Trace, "Scene Subsystem Deinitialized")

		m_Initialized = false;
	}


	bool SceneSubsystem::LoadScene(const Ref<Scene>& _scene)
	{
		// Check if the pointer is valid
		if (!_scene)
		{
			DE_LOG(LogSceneSubSystem, Error, "Load Scene: Invalid scene reference")
			return false;
		}

		// Check it isn't already loaded
		if (m_LoadedScenes.contains(_scene->GetSceneName()))
		{
			DE_LOG(LogSceneSubSystem, Error, "Load Scene: A scene name {} is already loaded", _scene->GetSceneName())
			return false;
		}

		// Load the scene
		if (!_scene->Load())
		{
			DE_LOG(LogSceneSubSystem, Critical, "Failed to load scene")
			return false;
		}

		m_LoadedScenes[_scene->GetSceneName()] = _scene;
		DE_LOG(LogSceneSubSystem, Trace, "Scene loaded: ", _scene->GetSceneName())

		return true;
	}
	
	void SceneSubsystem::UnloadScene(const std::string& _name)
	{
		if (const Ref<Scene>scene = m_LoadedScenes[_name])
		{
			// Unload the scene
			scene->Unload();
			m_LoadedScenes.erase(_name);

			DE_LOG(LogSceneSubSystem, Info, "Unloaded Scene: {}", _name)
			return;
		}

		DE_LOG(LogSceneSubSystem, Error, "Load Scene: Invalid scene name, or the scene isn't loaded")
	}

	void SceneSubsystem::OpenScene(const std::string& _name)
	{
		if (const Ref<Scene>scene = m_LoadedScenes[_name])
		{
			m_ActiveScene = scene;

			// Set dependencies with new scene pointer
			s_RendererSubsystem->SetActiveScene(m_ActiveScene);
			s_PhysicsSubsystem->SetActiveScene(m_ActiveScene);
			EditorSubsystem::Get()->SetActiveScene(m_ActiveScene);

			// Begin new scene
			m_ActiveScene->BeginScene();

			DE_LOG(LogSceneSubSystem, Info, "Activated Scene: {}", _name)
			return;
		}
	
		DE_LOG(LogSceneSubSystem, Error, "Cound't find Scene: {}", _name)
	}

	void SceneSubsystem::PlayScene()
	{
		if (m_ActiveScene)
		{
			// Check for Game Camera
			if(const Ref<Camera> camera = m_ActiveScene->GetGameCamera())
			{
				// Set the camera as the active camera
				m_ActiveScene->m_ActiveCamera = camera;
				DE_LOG(LogSceneSubSystem, Info, "Game Camera Found: {}", camera->GetName())
			}
			else
			{
				DE_LOG(LogSceneSubSystem, Warn, "No Game Camera found. Using Viewport Camera Instead")
			}

			m_ActiveScene->BeginPlay();

			
			DE_LOG(LogSceneSubSystem, Trace, "Scene Playing")
		}
	}

	void SceneSubsystem::StopScene()
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->EndPlay();
			m_ActiveScene->EndScene();

			UnloadScene(m_ActiveScene->GetSceneName());
			m_ActiveScene = nullptr;
			
			DE_LOG(LogSceneSubSystem, Trace, "Scene Stopped")

			Engine& engine = Engine::Get();
			engine.RestartScene();
		}
	}

	void SceneSubsystem::PauseScene()
	{
		DE_LOG(LogSceneSubSystem, Trace, "Scene Paused")
	}

	void SceneSubsystem::CleanRubbish()
	{
		// Cleanup rubbish objects here. TEMP loop, will be moved to a queue
		for (const auto& gameObject : m_ActiveScene->m_SceneObjects)
		{
			if (gameObject->IsRubbish())
			{
				// This will removec registered Components & other neccessary cleanups
				gameObject->EndScene();
				std::erase(m_ActiveScene->m_SceneObjects, gameObject);
			}
		}
	}

	void SceneSubsystem::Update(float _deltaTime)
	{
		// Instead of updating the object as a whole, we break it down into components and update them individually
		// Components should act as containers for the data, the logic should be implemented on the system side

		// Update Camera - This works regardless of the camer type (viewport/GameCamera)
		if (const Ref<Camera> cam = m_ActiveScene->GetViewportCamera())
		{
			cam->SetAspect(s_WindowSubsystem->GetWindow()->GetWindowSize());
			cam->Update(_deltaTime);
		}

		// Scene update implementation 
		m_ActiveScene->Update(_deltaTime);

		// Update Transform Components
		for (const auto& transform : m_TransformComponents)
		{
			if (transform)
			{
				glm::mat4& model = transform->GetModel();
				glm::vec3& scale = transform->GetScale();
				glm::vec3& position = transform->GetPosition();
				const glm::vec3& rotation = transform->GetRotation();

				// Update the Model matrix
				model = glm::translate(glm::mat4(1.0f), position);
				model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
				model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
				model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
				model = glm::scale(model, scale);
			}
		}

		// m_ActiveScene->m_ActiveCamera->m_RenderTexture->bind();

		// Lighting
		{
			Ref<GLShader> program = ResourceSubsystem::GetShader("DefaultShader");
			program->Bind();

			if (const Ref<DirectionalLight> dirLight = m_ActiveScene->m_DirLight)
			{
				const glm::vec3& lightDir = dirLight->GetLightDirection();
				const glm::vec3& lightColor = dirLight->GetLightColor();
				glUniform3f(program->GetUniform("u_DirLight.Base.Color"), lightColor.r, lightColor.g, lightColor.b);
				glUniform1f(program->GetUniform("u_DirLight.Base.AmbientIntensity"), dirLight->GetAmbientIntensity());
				glUniform1f(program->GetUniform("u_DirLight.Base.DiffuseIntensity"), dirLight->GetDiffuseIntensity());
				glUniform3f(program->GetUniform("u_DirLight.Direction"), lightDir.x, lightDir.y, lightDir.z);
			}

			glUniform1i(program->GetUniform("u_PointLightCount"), (int)m_ActiveScene->m_PointLights.size());
			glUniform1i(program->GetUniform("u_SpotLightCount"), (int)m_ActiveScene->m_SpotLights.size());


			for (int i = 0; i < (int)m_ActiveScene->m_PointLights.size(); i++)
			{
				const glm::vec3& lightCol = m_ActiveScene->m_PointLights[i]->GetLightColor();
				glUniform3f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Base.Color"), lightCol.r, lightCol.g, lightCol.b);
				glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Base.AmbientIntensity"), m_ActiveScene->m_PointLights[i]->GetAmbientIntensity());
				glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Base.DiffuseIntensity"), m_ActiveScene->m_PointLights[i]->GetDiffuseIntensity());

				const glm::vec3& pos = m_ActiveScene->m_PointLights[i]->GetTransformComponent()->GetPosition();
				glUniform3f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Position"), pos.x, pos.y, pos.z);
				glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Constant"), m_ActiveScene->m_PointLights[i]->GetConstant());
				glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Linear"), m_ActiveScene->m_PointLights[i]->GetLinear());
				glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Exponent"), m_ActiveScene->m_PointLights[i]->GetExponent());
			}

			for (int i = 0; i < (int)m_ActiveScene->m_SpotLights.size(); i++)
			{
				const glm::vec3& lightCol = m_ActiveScene->m_SpotLights[i]->GetLightColor();
				glUniform3f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Base.Color"), lightCol.r, lightCol.g, lightCol.b);
				glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Base.AmbientIntensity"),m_ActiveScene-> m_SpotLights[i]->GetAmbientIntensity());
				glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Base.DiffuseIntensity"),m_ActiveScene-> m_SpotLights[i]->GetDiffuseIntensity());

				const glm::vec3& pos = m_ActiveScene->m_SpotLights[i]->GetTransformComponent()->GetPosition();
				glUniform3f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Position"), pos.x, pos.y, pos.z);
				glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Constant"),m_ActiveScene-> m_SpotLights[i]->GetConstant());
				glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Linear"), m_ActiveScene->m_SpotLights[i]->GetLinear());
				glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Exponent"),m_ActiveScene-> m_SpotLights[i]->GetExponent());

				const glm::vec3& dir = m_ActiveScene->m_SpotLights[i]->GetDirection();
				glUniform3f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Direction"), dir.x, dir.y, dir.z);
				glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Edge"), m_ActiveScene->m_SpotLights[i]->GetProcessedEdge());
			}
			GLShader::Unbind();
		}

		// Update the GameObjects
		for (const auto& gameObject : m_ActiveScene->m_SceneObjects)
		{
			// Update the GameObject -  This will always be here
			gameObject->Update(_deltaTime);
			
			// TEMP Immediate Mode Rendering
			switch (static_cast<ViewportMode>(m_ViewportMode))
			{
				case ViewportMode::Default:
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

					if (RendererSubsystem::ValidateForDrawing(
						gameObject->GetRenderComponent(),
						gameObject->GetTransformComponent(),
						gameObject->GetMeshComponent()))
					{
						if (gameObject->GetRenderComponent()->IsVisible())
						{
							{
								s_RendererSubsystem->DrawImmediate(
									gameObject->GetRenderComponent(),
									gameObject->GetTransformComponent(),
									gameObject->GetMeshComponent());
							}
						}
					}

					// Draw Collider over gameobject if set to visible
					if (const Ref<Collider> collider = gameObject->GetCollider(); collider->GetRenderComponent()->IsVisible())
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

						if (RendererSubsystem::ValidateForDrawing(
							collider->GetRenderComponent(),
							gameObject->GetTransformComponent(),
							collider->GetMeshComponent()))
						{
							s_RendererSubsystem->DrawImmediate(
								collider->GetRenderComponent(),
								gameObject->GetTransformComponent(),
								collider->GetMeshComponent());
						}
					}
				}
					break;

				case ViewportMode::Wireframe:
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

					// Draw the GameObject
					s_RendererSubsystem->DrawImmediate(
						gameObject->GetRenderComponent(),
						gameObject->GetTransformComponent(),
						gameObject->GetMeshComponent());
				}
					break;

				case ViewportMode::Collider:
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

					// Draw the GameObject
					if (const Ref<Collider> collider = gameObject->GetCollider())
					{
						s_RendererSubsystem->DrawImmediate(
							collider->GetRenderComponent(),
							gameObject->GetTransformComponent(),
							gameObject->GetMeshComponent());
					}
				}
					break;

				default: DE_LOG(LogScene, Error, "Invalid Viewport") break;
			}
		}

		// m_ActiveScene->m_ActiveCamera->m_RenderTexture->unbind();
	}
}
