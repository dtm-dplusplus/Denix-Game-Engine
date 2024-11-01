#include "CPGScene.h"

#include "imgui.h"
#include "implot.h"
#include "CPG/Ray.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Denix/Engine.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "yaml-cpp/yaml.h"
#include "Denix/Reflection/ReflectionSubsystem.h"
#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Core/Timer.h"
#include "Denix/Core/TimerSubsystem.h"
#include "Denix/Video/Renderer/RendererSubsystem.h"

using namespace Denix;

// utility structure for realtime plot
struct ScrollingBuffer
{
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;

    ScrollingBuffer(int max_size = 2000)
    {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }

    void AddPoint(float x, float y)
    {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else
        {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }

    void Erase()
    {
        if (Data.size() > 0)
        {
            Data.shrink(0);
            Offset = 0;
        }
    }
};

struct ScrollingBufferDE
{
    int MaxSize;
    int Offset;
    std::vector<glm::vec2> Data;

    ScrollingBufferDE(int max_size = 2000)
    {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }

    void AddPoint(float x, float y)
    {
        if (Data.size() < MaxSize)
            Data.emplace_back(x, y);
        else
        {
            Data[Offset] = glm::vec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }

    void Erase()
    {
        if (Data.size() > 0)
        {
            Data.resize(0);
            Offset = 0;
        }
    }
};


CPGScene::CPGScene(const Ref<Asset>& _sceneAsset): Scene(_sceneAsset)
{
}

void CPGScene::Update(float _deltaTime)
{
    Scene::Update(_deltaTime);

    // This should be done on a timer to check for new assets
    ShowEngineContent = false;

    if (ImGui::Begin(m_SceneName.c_str()))
    {
        if(ImGui::CollapsingHeader("Dev Stuff", ImGuiTreeNodeFlags_DefaultOpen))
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
        }
        
        if (ImGui::CollapsingHeader("Thread", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Thread Count: %d", std::thread::hardware_concurrency());
            ImGui::SeparatorText("Scene Thread");
            ImGui::Checkbox("Scene Threaded", &SceneSubsystem::Get()->m_SceneThreaded);
        }

        if (ImGui::CollapsingHeader("Profiler", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const auto& profiles = ProfileSubsystem::Get()->GetProfiles();
            const float elaspedTime = TimerSubsystem::GetProgramElapsedTime();

            ImGui::DragInt("Max FPS", &TimerSubsystem::GetMaxFPS(), 1, 0, 240);
            ImGui::Text("Program time: %fms", TimerSubsystem::GetProgramElapsedTime());
            ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTime());
            ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());


            static float history = 5.0f;
            ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

            static bool showOtherProfiles = false;
            ImGui::Checkbox("Show Other Profiles", &showOtherProfiles);
            const Ref<Profile>& engprofile = TimerSubsystem::Get()->m_TimerProfile;
            engprofile->m_Buffer.AddPoint(elaspedTime, engprofile->GetDuration());

            if (showOtherProfiles)
            {
                if (ImPlot::BeginPlot("##Profiling", nullptr, "Frame Time (ms)",ImVec2(-1, 0), ImPlotFlags_None, ImPlotFlags_None, ImPlotAxisFlags_AutoFit))
                {
                    ImPlot::SetupAxisLimits(ImAxis_X1,elaspedTime - history, elaspedTime, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1,0.0f,engprofile->m_AverageDuration * 1.5f, ImGuiCond_Always);
                    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
                    for (const auto& [name, profile] : profiles)
                        ImPlot::PlotLine(name.c_str(), &profile->m_Buffer.Data[0].x, &profile->m_Buffer.Data[0].y, profile->m_Buffer.Data.size(), 0, profile->m_Buffer.Offset, 2*sizeof(float));
                    ImPlot::EndPlot();
                }
            }
            else
            {
                if (ImPlot::BeginPlot("##Profiling", nullptr, "Frame Time (ms)",ImVec2(-1, 0), ImPlotFlags_None, ImPlotFlags_None, ImPlotAxisFlags_AutoFit))
                {
                    ImPlot::SetupAxisLimits(ImAxis_X1,elaspedTime - history, elaspedTime, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1,0.0f,engprofile->m_AverageDuration * 1.5f, ImGuiCond_Always);
                    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
                    ImPlot::PlotLine("Engine", &engprofile->m_Buffer.Data[0].x, &engprofile->m_Buffer.Data[0].y, engprofile->m_Buffer.Data.size(), 0, engprofile->m_Buffer.Offset, 2*sizeof(float));
                    ImPlot::EndPlot();
                }
            }

            for (const auto& [name, profile] : profiles)
            {
                profile->m_Buffer.AddPoint(elaspedTime, profile->GetDuration());

                if (ImGui::TreeNode(name.c_str()))
                {
                    if (ImPlot::BeginPlot("##Profiling", nullptr, "Frame Time (ms)",ImVec2(-1, 0), ImPlotFlags_None, ImPlotFlags_None, ImPlotAxisFlags_AutoFit))
                    {
                        ImPlot::SetupAxisLimits(ImAxis_X1,elaspedTime - history, elaspedTime, ImGuiCond_Always);
                        ImPlot::SetupAxisLimits(ImAxis_Y1,profile->m_AverageDuration * 0.5f,profile->m_MaximumDuration * 1.25f, ImGuiCond_Always);
                        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
                        ImPlot::PlotLine(name.c_str(), &profile->m_Buffer.Data[0].x, &profile->m_Buffer.Data[0].y, profile->m_Buffer.Data.size(), 0, profile->m_Buffer.Offset, 2*sizeof(float));
                        ImPlot::EndPlot();
                    }
                    ImGui::Text("Frame Percentage: %.2f%%", profile->m_FramePercentage * 100.0f);
                    ImGui::Text("Duration: %fms", profile->GetDuration());
                    ImGui::Text("Minimum Duration: %fms", profile->m_MinimumDuration);
                    ImGui::Text("Maximum Duration: %fms", profile->m_MaximumDuration);
                    ImGui::Text("Average Duration: %fms", profile->m_AverageDuration);
                    if (ImGui::DragInt("Average Duration Count", &profile->m_AverageDurationCount, 1.0f, 3, 100))
                    {
                        profile->m_DurationRecords.resize(profile->m_AverageDurationCount);
                    }
                    ImGui::TreePop();
                }
            }
            
            if (ImGui::CollapsingHeader("Reflection"))
            {
                for (const auto& key : ReflectionSubsystem::GetCreateFuncs() | std::views::keys)
                {
                    ImGui::Text(key.c_str());
                }
            }

            if (ImGui::CollapsingHeader("Engine Config"), ImGuiTreeNodeFlags_DefaultOpen)
            {
                ImGui::Text("Project Name: %s", FileSubsystem::GetProjectName().c_str());
                ImGui::Text("Project Root: %s", FileSubsystem::GetProjectRoot().c_str());
                ImGui::Text("User Content Root: %s", FileSubsystem::GetContentRoot().c_str());
                ImGui::Checkbox("Show engine content", &ShowEngineContent);

                if (ImGui::Button("Save Config"))
                {
                    Engine::Get().SaveConfig();
                }
            }

            if (ImGui::CollapsingHeader("Assets"))
            {
                if (ImGui::TreeNode("Materials"))
                {
                    for (const auto& mat : ResourceSubsystem::GetMaterialStore())
                    {
                        ImGui::Text(mat.second->GetAsset()->GetAssetName().c_str());
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Scenes"))
                {
                    if (ImGui::Button("Save Scene"))
                    {
                        SceneSubsystem::SerializeScene(this);

                        for (const auto& mat : ResourceSubsystem::GetMaterialStore())
                        {
                            // Save Changes to asset - This should be done in the editor
                            YAML::Emitter matAsssetEmitter;
                            matAsssetEmitter << YAML::Comment("DE_ASSET: Material");
                            matAsssetEmitter << YAML::BeginMap;
                            mat.second->Serialize(matAsssetEmitter);
                            matAsssetEmitter << YAML::EndMap;

                            FileSubsystem::WriteFile(mat.second->GetAsset()->GetAssetPath(), matAsssetEmitter.c_str());
                            DE_LOG(LogScene, Info, "Serialized Material");
                        }
                    }
                    for (const auto& scene : ResourceSubsystem::GetSceneStore())
                    {
                        ImGui::Text(scene->GetAssetName().c_str());

                        if (ImGui::Button("Set as startup scene"))
                        {
                            Engine::Get().SetStartupScene(m_SceneAsset);
                        }
                        if (ImGui::Button("Open"))
                        {
                            SceneSubsystem::OpenScene(scene);
                        }
                        ImGui::Text("Asset Path: %s", scene->GetAssetPath().c_str());
                    }
                    ImGui::TreePop();
                }
            }

            ImGui::End();
        }
    }
}
