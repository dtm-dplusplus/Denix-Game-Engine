#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "AssetBrowserWidget.h"
#include "Denix/UI/UISubsystem.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Scene/Camera.h"
#include "Denix/Scene/Actor.h"
#include "Denix/Editor/Widget/ShaderEditor.h"

Denix::AssetBrowserWidget::AssetBrowserWidget(): EditorWidget({"AssetBrowser"})
{
}

void Denix::AssetBrowserWidget::Update(float _deltaTime)
{
    EditorWidget::Update(_deltaTime);

    ImGui::SetNextWindowDockID(UISubsystem::Get()->DockDownID, ImGuiCond_Appearing);
    bool open = !IsRubbish();
    ImGui::Begin("Asset Browser", &open);
  
			
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
            ImGui::Text("Asset Path: %s", scene->GetAssetPath().c_str());
        }
        ImGui::TreePop();
    }
    ImGui::End();

    if(!open) MarkRubbish();
}

