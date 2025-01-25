
#include "AssetBrowserWidget.h"

#include "imgui.h"
#include "Denix/UI/UISubsystem.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/Scene/Camera.h"
#include "Denix/Scene/Actor.h"

Denix::AssetBrowserWidget::AssetBrowserWidget(): EditorWidget({"AssetBrowser"})
{
}

void Denix::AssetBrowserWidget::Update(float _deltaTime)
{
    EditorWidget::Update(_deltaTime);

    ImGui::SetNextWindowDockID(EditorSubsystem::GetDockDownID(), ImGuiCond_Appearing);
    ImGui::Begin("Asset Browser", &m_IsOpen);
  
			
    if (ImGui::TreeNode("Materials"))
    {
        for (const auto& mat : AssetSubsystem::GetMaterialStore())
        {
            ImGui::Text(mat.second->GetAsset()->GetAssetName().c_str());
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Scenes"))
    {
                
        for (const auto& scene : AssetSubsystem::GetSceneStore())
        {
            ImGui::Text(scene->GetAssetName().c_str());
            ImGui::Text("Asset Path: %s", scene->GetRelativePath().c_str());
        }
        ImGui::TreePop();
    }
    ImGui::End();

    if(!open) MarkRubbish();
}

