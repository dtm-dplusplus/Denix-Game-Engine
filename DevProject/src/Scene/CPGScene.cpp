#include "CPGScene.h"

#include "imgui.h"
#include "CPG/Ray.h"

using namespace Denix;

CPGScene::CPGScene(const Ref<Asset>& _sceneAsset): Scene(_sceneAsset)
{
}

void CPGScene::Update(float _deltaTime)
{
    Scene::Update(_deltaTime);
    
}

void CPGScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    if (ImGui::Begin(m_SceneName.c_str()))
    {
        if(ImGui::CollapsingHeader("Ray Tracing", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::SeparatorText("Ray Tracing");
            if (ImGui::Button("Spawn Ray"))
            {
                const glm::vec3& camPos = m_ActiveCamera->GetTransformComponent()->GetPosition();
                const glm::vec3& camForward = m_ActiveCamera->GetCameraFront();
                const glm::vec3& camRight = m_ActiveCamera->m_CameraRight;
                glm::vec3 rot = m_ActiveCamera->GetTransformComponent()->GetRotation();
                rot  = {rot.x, -rot.y, 90.0f};
                m_Ray = SpawnGameObject<Ray>(camPos, rot);
            }
            ImGui::End();
        }
    }
}