#include "ShaderEditor.h"

#include "Denix/Video/GL/Shader.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Denix/Asset/AssetSubsystem.h"

namespace Denix
{
    ShaderEditor::ShaderEditor(const Ref<Shader>& _shader): ShaderObject(_shader), IsOpen(true)
    {
        
    }

    void ShaderEditor::Update()
    {
        ImGui::Begin("Shader Editor", &IsOpen);
        ImGui::BeginTabBar("Shader Editor Tabs");
        if (ShaderObject)
        {
            for (auto& shaderSource : ShaderObject->GetShaderSources())
            {
                if (ImGui::BeginTabItem(shaderSource.FileName.c_str()))
                {
                    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput |
                        ImGuiInputTextFlags_CtrlEnterForNewLine ;
                    if(ImGui::Button("Recompile Shader"))
                    {
                        AssetSubsystem::ReloadShader(ShaderObject);
                    }
                    ImGui::BeginChild(shaderSource.FileName.c_str());
                    ImGui::InputTextMultiline("##source", &shaderSource.Source, ImGui::GetWindowSize(), flags);
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }
            }
        }
        ImGui::EndTabBar();
        ImGui::End();
    }
}
