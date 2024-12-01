#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "AddActorWidget.h"

#include "Denix/UI/UISubsystem.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Scene/Actor.h"
#include "Denix/Scene/Object/Shapes/Shapes.h"
#include "Denix/Scene/Object/Light/LightObject.h"

Denix::AddActorWidget::AddActorWidget(const WRef<Scene>& _scene)
{
	m_SceneRef = _scene;
	m_CreatedActor = false;
}

void Denix::AddActorWidget::Update(float _deltaTime)
{
    ImGui::BeginChild("AddObject", ImVec2(100, 25));
		bool m_CreatedActor = false;

		const static std::string shapeNames[] = { "Plane", "Cube", "Sphere"};
		const static std::string lightNames[] = { "Directional Light", "Point Light", "Spot Light" };

		if (ImGui::Button("Add"))
			ImGui::OpenPopup("add_object_popup");

		if (ImGui::BeginPopup("add_object_popup"))
		{
			if (ImGui::BeginMenu("Shapes"))
			{
				for (auto& name : shapeNames)
				{
					if (ImGui::MenuItem(name.c_str()))
					{
						m_CreatedActor = true;
						if (name == "Plane")
						{
							m_SceneRef.lock()->SpawnActor<Plane>();
						}
						else if (name == "Cube")
						{
							m_SceneRef.lock()->SpawnActor<Cube>();
						}
						else if (name == "Sphere")
						{
							m_SceneRef.lock()->SpawnActor<Sphere>();
						}
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Lights"))
			{
				for (auto& name : lightNames)
				{
					if (ImGui::MenuItem(name.c_str()))
					{
						m_CreatedActor = true;
						if (name == lightNames[0])
						{
							m_SceneRef.lock()->SpawnActor<DirectionalLight>();
						}
						else if (name == lightNames[1])
						{
							m_SceneRef.lock()->SpawnActor<PointLight>();
						}
						else if (name == lightNames[2])
						{
							m_SceneRef.lock()->SpawnActor<SpotLight>();
						}
					}
				}
				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		ImGui::EndChild();
		ImGui::Separator();
}
