#include "ShaderEditor.h"

#include "Denix/Video/GL/Shader.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Denix/Asset/AssetSubsystem.h"

namespace Denix
{
    ShaderEditor::ShaderEditor(const Ref<Shader>& _shader): EditorWidget({"Shader Editor" + _shader->GetAssetName()}),
                                                            m_ShaderRef(_shader)
    {
    }

    void ShaderEditor::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        EditorWidget::Update(_deltaTime, _waitCounter);
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Appearing);
        if (ImGui::Begin("Shader Editor", &m_IsOpen))
        {
            ImGui::BeginTabBar("Shader Editor Tabs");

            if (const auto shader = m_ShaderRef.lock())
            {
                for (auto& shaderSource : shader->GetShaderSources())
                {
                    if (ImGui::BeginTabItem(shaderSource.FileName.c_str()))
                    {
                        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput |
                            ImGuiInputTextFlags_CtrlEnterForNewLine;
                        if (ImGui::Button("Recompile Shader"))
                        {
                            AssetSubsystem::ReloadShader(shader);
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
}
