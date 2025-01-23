
#include "SceneOrganizerWidget.h"

#include "ActorDetailsWidget.h"
#include "imgui.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/UI/UISubsystem.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Scene/Actor.h"
#include "Denix/Editor/Widget/Scene/AddActorWidget.h"
#include "Denix/Editor/Widget/Scene/SceneSettingsWidget.h"

namespace Denix
{
    SceneOrganizerWidget::SceneOrganizerWidget(const WRef<Scene>& _scene) : SceneEditorWidget({"SceneOrganizer"}, _scene)
    {
        m_SceneRef = _scene;
        m_SelectionIndex = _scene.lock()->GetSceneActors().size() - 1;
        m_AddActorWidget = MakeRef<AddActorWidget>(_scene);
        m_SceneSettingsWidget = MakeRef<SceneSettingsWidget>(_scene);
    }

    void SceneOrganizerWidget::Update(float _deltaTime)
    {
        EditorWidget::Update(_deltaTime);

        if(!m_SceneRef.lock()) return;

        std::vector<Ref<Actor>>& sceneObjects = m_SceneRef.lock()->GetSceneActors();

        if (sceneObjects.empty())
        {
            ResetSelection();
        }
        
        // Scene Objects
        ImGui::SetNextWindowDockID(EditorSubsystem::GetDockLeftID(), ImGuiCond_Appearing);
        ImGui::Begin(GetName().c_str());
        ImGui::SeparatorText(m_SceneRef.lock()->GetName().c_str());

        // Viewport Camera
        ImGui::SeparatorText("Viewport Camera");
        ActorDetailsWidget::CameraWidget(m_SceneRef.lock()->GetViewportCamera());

        // Update AddActorWidget. Set the selected object to the last object created
        m_AddActorWidget->Update(_deltaTime);
        if (m_AddActorWidget->m_CreatedActor)
        {
            m_AddActorWidget->m_CreatedActor = false;
            SetSelection(sceneObjects.size() - 1);
        }
        
        for (int i = 0; i < sceneObjects.size(); i++)
        {
            // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
            ImGui::PushID(sceneObjects[i]->GetName().c_str());
            if (ImGui::Selectable(sceneObjects[i]->GetName().c_str(), m_SelectionIndex == i))
            {
                m_SelectionIndex = i;
                DE_LOG(LogEditor, Trace, "Selected Object: {0}", sceneObjects[i]->GetName());
            }
            ImGui::PopID();
            
            if (ImGui::BeginPopupContextItem()) //uses last item id as popup id
            {
                m_SelectionIndex = i;
                DE_LOG(LogEditor, Trace, "Selected Object: {0}", sceneObjects[i]->GetName());

                // Delete Button
                if (ImGui::Button("Delete"))
                {
                    sceneObjects[i]->Destroy();
                    ImGui::CloseCurrentPopup();
                    ResetSelection();
                }
                ImGui::EndPopup();
            }
        }
        
        ImGui::End();

        // Scene Settings
        //m_SceneSettingsWidget->Update(_deltaTime);
    }

    void SceneOrganizerWidget::SceneChangedEvent(const WRef<Scene>& _scene)
    {
        SceneEditorWidget::SceneChangedEvent(_scene);
        m_AddActorWidget->m_SceneRef = _scene;
        ResetSelection();
    }

    void SceneOrganizerWidget::ResetSelection()
    {
        m_SelectionIndex = -1;
    }

    void SceneOrganizerWidget::SetSelection(const int _index)
    {
        // Validate index
        if (_index < 0 || _index >= m_SceneRef.lock()->GetSceneActors().size())
        {
            m_SelectionIndex = -1;
            return;
        }

        m_SelectionIndex = _index;
    }

    Ref<Actor> SceneOrganizerWidget::GetSelectedObject() const
    {
        if (!ValidateSelection()) return nullptr;
        
        if (Ref<Actor> selectedObject = m_SceneRef.lock()->GetSceneActors().at(m_SelectionIndex))
        {
            return selectedObject;
        }

        return nullptr;
    }

    bool SceneOrganizerWidget::ValidateSelection() const
    {
        return m_SelectionIndex >= 0 && m_SceneRef.lock()->GetSceneActors().size();
    }
}
