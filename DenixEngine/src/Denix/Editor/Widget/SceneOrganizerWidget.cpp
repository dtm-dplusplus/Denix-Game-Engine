#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "SceneOrganizerWidget.h"

#include "Denix/UI/UISubsystem.h"

#include "Denix/Scene/Scene.h"
#include "Denix/Scene/GameObject.h"


namespace Denix
{
    SceneOrganizerWidget::SceneOrganizerWidget(const WRef<Scene>& _scene) : EditorWidget("SceneOrganizer")
    {
        m_SceneRef = _scene;
        m_SelectedObjectIndex = -1;
    }

    void SceneOrganizerWidget::Update(float _deltaTime)
    {
        EditorWidget::Update(_deltaTime);

        if(!m_SceneRef.lock()) return;

        std::vector<Ref<GameObject>>& sceneObjects = m_SceneRef.lock()->GetSceneObjects();
        
        // Scene Objects
        ImGui::BeginChild("SceneOrganizer", ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
        for (int i = 0; i < sceneObjects.size(); i++)
        {
            // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
            if (ImGui::Selectable(sceneObjects[i]->GetName().c_str(), m_SelectedObjectIndex == i))
            {
                m_SelectedObjectIndex = i;
            }

            if (ImGui::BeginPopupContextItem()) //uses last item id as popup id
            {
                m_SelectedObjectIndex = i;

                // Delete Button
                if (ImGui::Button("Delete"))
                {
                    sceneObjects[i]->Destroy();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
        ImGui::EndChild();
    }

    void SceneOrganizerWidget::SetObjectSelection(int _index)
    {
        // Validate index
        if (_index < 0 || _index >= m_SceneRef.lock()->GetSceneObjects().size())
        {
            m_SelectedObjectIndex = -1;
            return;
        }
    }

    Ref<GameObject> SceneOrganizerWidget::GetSelectedObject() const
    {
        if (m_SelectedObjectIndex < 0 || m_SelectedObjectIndex >= m_SceneRef.lock()->GetSceneObjects().size())
        {
            return nullptr;
        }

        return m_SceneRef.lock()->GetSceneObjects()[m_SelectedObjectIndex];
    }
}
