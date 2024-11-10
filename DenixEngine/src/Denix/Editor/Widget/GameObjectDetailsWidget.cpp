#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "GameObjectDetailsWidget.h"
#include "Denix/Scene/GameObject.h"
#include "Denix/UI/UISubsystem.h"

Denix::GameObjectDetailsWidget::GameObjectDetailsWidget(): EditorWidget("GameObjectDetails")
{
}

/*
void Denix::GameObjectDetailsWidget::Update(float _deltaTime)
{
    EditorWidget::Update(_deltaTime);

    ImGui::SetNextWindowSize(ImVec2((ImGui::GetWindowWidth() / 5), ImGui::GetWindowHeight()), ImGuiCond_Appearing);
    //ImGui::SetNextWindowPos(ImVec2((WinX / 6), MenuBarHeight), ImGuiCond_Appearing);

    ImGui::SetNextWindowDockID(UISubsystem::Get()->DockRightID);
    ImGui::Begin(m_GameObjectRef.lock() ? m_GameObjectRef.lock()->GetName().c_str() : "GameObject Details");
        /*if (m_ObjectSelection >= 0 && m_ObjectSelection < m_ActiveScene->m_SceneObjects.size())
        {
            const Ref<GameObject> selectedObject = m_ActiveScene->m_SceneObjects[m_ObjectSelection];

            ImGui::SeparatorText(selectedObject->GetName().c_str());

            TransformWidget(selectedObject);
            CameraWidget(selectedObject);
            LightWidget(selectedObject);
            PhysicsWidget(selectedObject);
            CollisionWidget(selectedObject);
            RenderWidget(selectedObject);
            MeshWidget(selectedObject);
        }#1#
        
    ImGui::End();
}
*/
