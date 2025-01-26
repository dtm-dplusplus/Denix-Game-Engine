
#include "AddActorWidget.h"

#include "imgui.h"
#include "Denix/UI/UISubsystem.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Scene/Actor.h"
#include "Denix/Scene/Actor/Shapes.h"

Denix::AddActorWidget::AddActorWidget(const WRef<Scene>& _scene): EditorWidget({"AddActor"})
{
	m_SceneRef = _scene;
	m_CreatedActor = false;
}

void Denix::AddActorWidget::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    ImGui::BeginChild("AddObject", ImVec2(100, 25));

		const static std::string shapeNames[] = { "Plane", "Cube", "Sphere"};

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

			ImGui::EndPopup();
		}

		ImGui::EndChild();
		ImGui::Separator();
}
