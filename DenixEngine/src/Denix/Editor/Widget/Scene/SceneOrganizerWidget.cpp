#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "SceneOrganizerWidget.h"

#include "Denix/UI/UISubsystem.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Scene/GameObject.h"
#include "Denix/Editor/Widget/Scene/AddGameObjectWidget.h"
#include "Denix/Editor/Widget/Scene/SceneSettingsWidget.h"

namespace Denix
{
    SceneOrganizerWidget::SceneOrganizerWidget(const WRef<Scene>& _scene) : SceneEditorWidget({"SceneOrganizer"}, _scene)
    {
        m_SceneRef = _scene;
        m_SelectionIndex = -1;
        m_AddGameObjectWidget = MakeRef<AddGameObjectWidget>(_scene);
        m_SceneSettingsWidget = MakeRef<SceneSettingsWidget>(_scene);
    }

    void SceneOrganizerWidget::Update(float _deltaTime)
    {
        EditorWidget::Update(_deltaTime);

        if(!m_SceneRef.lock()) return;

        std::vector<Ref<GameObject>>& sceneObjects = m_SceneRef.lock()->GetSceneObjects();
        
        // Scene Objects
        ImGui::SetNextWindowDockID(UISubsystem::Get()->DockLeftID, ImGuiCond_Appearing);
        ImGui::Begin(GetName().c_str());

        // Update AddGameObjectWidget. Set the selected object to the last object created
        m_AddGameObjectWidget->Update(_deltaTime);
        if (m_AddGameObjectWidget->m_CreatedGameObject)
        {
            m_AddGameObjectWidget->m_CreatedGameObject = false;
            SetSelection(sceneObjects.size() - 1);
        }
        
        for (int i = 0; i < sceneObjects.size(); i++)
        {
            // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
            if (ImGui::Selectable(sceneObjects[i]->GetName().c_str(), m_SelectionIndex == i))
            {
                m_SelectionIndex = i;
                DE_LOG(LogEditor, Trace, "Selected Object: {0}", sceneObjects[i]->GetName());
            }

            if (ImGui::BeginPopupContextItem()) //uses last item id as popup id
            {
                m_SelectionIndex = i;
                DE_LOG(LogEditor, Trace, "Selected Object: {0}", sceneObjects[i]->GetName());

                // Delete Button
                if (ImGui::Button("Delete"))
                {
                    sceneObjects[i]->Destroy();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
        
        ImGui::End();

        // Scene Settings
        m_SceneSettingsWidget->Update(_deltaTime);
    }

    void SceneOrganizerWidget::SceneChangedEvent(const WRef<Scene>& _scene)
    {
        SceneEditorWidget::SceneChangedEvent(_scene);
        ResetSelection();
    }

    void SceneOrganizerWidget::ResetSelection()
    {
        m_SelectionIndex = -1;
    }

    void SceneOrganizerWidget::SetSelection(const int _index)
    {
        // Validate index
        if (_index < 0 || _index >= m_SceneRef.lock()->GetSceneObjects().size())
        {
            m_SelectionIndex = -1;
            return;
        }

        m_SelectionIndex = _index;
    }

    Ref<GameObject> SceneOrganizerWidget::GetSelectedObject() const
    {
        if (ValidateSelection())
        {
            return m_SceneRef.lock()->GetSceneObjects()[m_SelectionIndex];
        }

        return nullptr;
    }

    bool SceneOrganizerWidget::ValidateSelection() const
    {
        return m_SelectionIndex >= 0 && m_SceneRef.lock()->GetSceneObjects().size() -1;
    }
}
