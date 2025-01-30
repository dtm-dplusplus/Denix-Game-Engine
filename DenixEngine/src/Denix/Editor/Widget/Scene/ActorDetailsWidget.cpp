#include "ActorDetailsWidget.h"

#include "imgui.h"
#include "Denix/UI/UISubsystem.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/Scene/Actor/Camera.h"
#include "Denix/Scene/Actor.h"
#include "Denix/Editor/Widget/ShaderEditor.h"


namespace Denix
{
    class ShaderEditor;
}

Denix::ActorDetailsWidget::ActorDetailsWidget(): EditorWidget({"ActorDetails"})
{
}

void Denix::ActorDetailsWidget::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    EditorWidget::Update(_deltaTime, _waitCounter);

    //ImGui::SetNextWindowSize(ImVec2((ImGui::GetWindowWidth() / 5), ImGui::GetWindowHeight()), ImGuiCond_Appearing);
    //ImGui::SetNextWindowPos(ImVec2((WinX / 6), MenuBarHeight), ImGuiCond_Appearing);

    ImGui::SetNextWindowDockID(EditorSubsystem::GetDockRightID(), ImGuiCond_Appearing);
    ImGui::Begin("Actor Details");
    //ImGui::SetWindowDock(ImGui::GetCurrentWindow(), UISubsystem::Get()->DockRightID, ImGuiCond_Appearing);

    if (const Ref<Actor> actorRef = m_ActorRef.lock())
    {
        ImGui::SeparatorText(actorRef->GetName().c_str());

        // Component Preview
        ImGui::SeparatorText("Components");
        ImGui::BeginChild("Component List", ImVec2(ImGui::GetWindowWidth(), 100), true);
        for (const auto& name : actorRef->GetComponentMap() | std::views::keys)
        {
            ImGui::Text(name.c_str());
        }

        ImGui::EndChild();

        // Component Widgets
        TransformWidget(actorRef);
        CameraWidget(actorRef);
        PhysicsWidget(actorRef);
        RenderWidget(actorRef);
        MeshWidget(actorRef);
    }

    ImGui::End();
}

void Denix::ActorDetailsWidget::TransformWidget(const Ref<Actor>& _actor) const
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
    if (ImGui::CollapsingHeader("Transform Component"))
    {
        const Ref<TransformComponent> transform = _actor->GetTransformComponent();

        if (ImGui::DragFloat3("Position", &transform->GetPosition()[0], m_DragSpeed))
            _actor->GetPhysicsComponent()->m_AttributeFlags |= PHYSICS_SHAPE;

        ImGui::SameLine();
        if (ImGui::ArrowButton("##ResetPosition", ImGuiDir_Left)) transform->SetPosition(glm::vec3(0.f));
        ImGui::SetItemTooltip("Reset");

        ImGui::DragFloat3("Rotation", &transform->GetRotation()[0], m_DragSpeed);
        ImGui::SameLine();
        if (ImGui::ArrowButton("##ResetRotation", ImGuiDir_Left)) transform->SetRotation(glm::vec3(0.f));
        ImGui::SetItemTooltip("Reset");

        if (ImGui::DragFloat3("Scale", &transform->GetScale()[0], m_DragSpeed, 0.01f, FLT_MAX))
            _actor->GetPhysicsComponent()->m_AttributeFlags |= PHYSICS_SHAPE;

        ImGui::SameLine();
        if (ImGui::ArrowButton("##ResetScale", ImGuiDir_Left)) transform->SetScale(glm::vec3(1.f));
        ImGui::SetItemTooltip("Reset");
    }
}

void Denix::ActorDetailsWidget::PhysicsWidget(const Ref<Actor>& _selectedObject) const
{
    if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const Ref<PhysicsComponent> comp = _selectedObject->GetPhysicsComponent();

        // Physics Simulation
        if (ImGui::Checkbox("Simulate Physics", &comp->m_SimulatePhysics)) comp->m_AttributeFlags |= PHYSICS_SIMULATE;

        if (ImGui::Checkbox("Simulate Collision", &comp->m_CollisionDetectionEnabled)) comp->m_AttributeFlags |=
            PHYSICS_COLLISION;

        // Mass
        if (ImGui::DragFloat("Mass", &comp->m_Mass, m_DragSpeed, 0.0f, FLT_MAX)) comp->m_AttributeFlags |= PHYSICS_MASS;

        // Linear Drag
        if (ImGui::DragFloat("Linear Drag", &comp->m_LinearDrag, m_DragSpeed, 0.0f, FLT_MAX)) comp->m_AttributeFlags |=
            PHYSICS_LINEAR_DRAG;

        // Angular Drag
        if (ImGui::DragFloat("Angular Drag", &comp->m_AngularDrag, m_DragSpeed, 0.0f, FLT_MAX)) comp->m_AttributeFlags
            |= PHYSICS_ANGULAR_DRAG;

        // Elasticity
        if (ImGui::DragFloat("Elasticity", &comp->m_Elasticity, m_DragSpeed, 0.0f, 1.0f)) comp->m_AttributeFlags |=
            PHYSICS_MATERIAL;

        // Static Friction
        if (ImGui::DragFloat("Static Friction", &comp->m_StaticFriction, m_DragSpeed, 0.0f, 1.0f)) comp->
            m_AttributeFlags |= PHYSICS_MATERIAL;

        // Dynamic Friction
        if (ImGui::DragFloat("Dynamic Friction", &comp->m_DynamicFriction, m_DragSpeed, 0.0f, 1.0f)) comp->
            m_AttributeFlags |= PHYSICS_MATERIAL;

        if (ImGui::TreeNode("Advanced Settings"))
        {
            // Impulse Response
            ImGui::Checkbox("Impulse Resonses", &comp->m_ImpulseEnabled);
            ImGui::TreePop();
        }

        // Viewable Properties
        ImGui::Text("Force				x: %.2f y: %.2f z: %.2f", comp->m_Force.x, comp->m_Force.y, comp->m_Force.z);
        ImGui::Text("Velocity			x: %.2f y: %.2f z: %.2f", comp->m_Velocity.x, comp->m_Velocity.y, comp->m_Velocity.z);
        ImGui::Text("Acceleration		x: %.2f y: %.2f z: %.2f", comp->m_Acceleration.x, comp->m_Acceleration.y, comp->m_Acceleration.z);
        ImGui::Text("Angular Velocity	x: %.2f y: %.2f z: %.2f", comp->m_AngularVelocity.x, comp->m_AngularVelocity.y, comp->m_AngularVelocity.z);
        ImGui::Text("Inertia Tensor		x: %.2f y: %.2f z: %.2f", comp->m_InertiaTensor.x, comp->m_InertiaTensor.y, comp->m_InertiaTensor.z);
    }
}

void Denix::ActorDetailsWidget::RenderWidget(const Ref<Actor>& _selectedObject)
{
    if (ImGui::CollapsingHeader("Rendering", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const Ref<RenderComponent> render = _selectedObject->GetRenderComponent();

        ImGui::Checkbox("Visible", &render->m_IsVisible);
        MaterialWidget(_selectedObject);
    }
}

void Denix::ActorDetailsWidget::MaterialWidget(const Ref<Actor>& _selectedObject)
{
    Ref<RenderComponent> rendComp = _selectedObject->GetRenderComponent();

    ImGui::SeparatorText("Material");

    // Material Settings
    if (Ref<Material> mat = rendComp->GetMaterial())
    {
        // Material Properties
        // Color or Texture selectable
        {
            ImGui::Text("Base Color");
            ImGui::ColorEdit3("Base Color", &mat->m_BaseColor[0]);
            ImGui::Checkbox("Multiply Base Color", &mat->m_MultiplyBase);
            ImGui::DragFloat("Texture Tiling", &mat->m_TextureTiling, m_DragSpeed, 0.1f, 1000.f);
            TextureSelectionWidget(mat);
            if (ImGui::Button("Clear Texture"))
            {
                mat->ClearBaseTexture();
            }
        }

        ImGui::Separator();
        if (Ref<Shader> shader = mat->GetShader())
        {
            ImGui::Text("Shader: %s", shader->GetAssetName().c_str());
            ImGui::Text("Shader Directory: %s", shader->GetDirectoryName().c_str());
            ImGui::Text("Shader ID: %d", shader->GetGL_ID());
            ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
            if (ImGui::TreeNode("Shader Sources"))
            {
                for (const auto& source : shader->GetShaderSources())
                {
                    ImGui::TextWrapped("File: %s", source.FileName.c_str());
                    ImGui::TextWrapped("Type: %s", GetShaderTypeToString(source.Type).c_str());
                }
                ImGui::TreePop();
            }
            if (ImGui::Button("Edit Shader"))
            {
                EditorSubsystem::AddEditorWidget<ShaderEditor>(shader);
            }
        }
    }
}

void Denix::ActorDetailsWidget::TextureSelectionWidget(const Ref<Material>& _material)
{
    Ref<Texture>& texture = _material->GetBaseTexture();
    std::string preview = "None";

    // Texture Preview
    if (texture)
    {
        preview = texture->GetAssetFileName();
        if (unsigned int id = texture->GetTextureID())
        {
            ImGui::Image((void*)static_cast<intptr_t>(id), ImVec2(100, 100));
            ImGui::SameLine();
        }

        //	// Texture Info
        ImGui::SeparatorText("Texture Info");
        ImGui::Text("Texture ID: %d", texture->GetTextureID());
        ImGui::TextWrapped("File Path : % s", texture->GetRelativePath().c_str());
        ImGui::Text("Size = %d x %d", texture->GetWidth(), texture->GetHeight());
        ImGui::Separator();

        //	// Texture Settings
        TextureSettings& texSettings = _material->m_TextureSettings;
        if (ImGui::Combo("Wrap Mode", &texSettings.WrapValue,
                         "GL_REPEAT\0GL_MIRRORED_REPEAT\0GL_CLAMP_TO_EDGE\0GL_CLAMP_TO_BORDER\0\0"))
        {
            if (texSettings.WrapValue == 0) texSettings.WrapMode = GL_REPEAT;
            else if (texSettings.WrapValue == 1) texSettings.WrapMode = GL_MIRRORED_REPEAT;
            else if (texSettings.WrapValue == 2) texSettings.WrapMode = GL_CLAMP_TO_EDGE;
            else if (texSettings.WrapValue == 3) texSettings.WrapMode = GL_CLAMP_TO_BORDER;
        }

        // Texture Filter
        if (ImGui::Combo("Filter", &texSettings.FilterValue,
                         "GL_NEAREST\0GL_LINEAR\0GL_NEAREST_MIPMAP_NEAREST\0GL_LINEAR_MIPMAP_NEAREST\0GL_NEAREST_MIPMAP_LINEAR\0GL_LINEAR_MIPMAP_LINEAR\0\0"))
        {
            if (texSettings.FilterValue == 0) texSettings.FilterMode = GL_NEAREST;
            else if (texSettings.FilterValue == 1) texSettings.FilterMode = GL_LINEAR;
            else if (texSettings.FilterValue == 2) texSettings.FilterMode = GL_NEAREST_MIPMAP_NEAREST;
            else if (texSettings.FilterValue == 3) texSettings.FilterMode = GL_LINEAR_MIPMAP_NEAREST;
            else if (texSettings.FilterValue == 4) texSettings.FilterMode = GL_NEAREST_MIPMAP_LINEAR;
            else if (texSettings.FilterValue == 5) texSettings.FilterMode = GL_LINEAR_MIPMAP_LINEAR;
        }
    }

    // Texture Selection
    if (ImGui::BeginCombo("##TextureSelection", preview.c_str(), ImGuiComboFlags_WidthFitPreview))
    {
        for (auto& snd : AssetSubsystem::GetTextureStore() | std::views::values)
        {
            ImGui::PushID(snd->GetAssetFileName().c_str());
            ImGui::Image((void*)static_cast<intptr_t>(snd->GetTextureID()), ImVec2(100, 100));
            ImGui::SameLine();
            if (ImGui::Selectable(snd->GetAssetFileName().c_str(), false,
                                  ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap,
                                  ImVec2(250, 100)))
            {
                _material->SetBaseTexture(snd);
                _material->CheckBaseType();
            }
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }
}

void Denix::ActorDetailsWidget::ShaderSelectionWidget(Ref<Material>& _material)
{
    if (!_material) return;

    ImGui::SeparatorText("Shader");
    if (Ref<Shader> shader = _material->GetShader())
    {
        if (ImGui::BeginCombo("##ShaderName", shader->GetDirectoryName().c_str()))
        {
            for (auto& [fst, snd] : AssetSubsystem::GetShaderStore())
            {
                ImGui::PushID(fst.c_str());
                if (ImGui::Selectable(fst.c_str()))
                {
                    _material->SetShader(snd);
                }
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
    }
}

void Denix::ActorDetailsWidget::MeshWidget(const Ref<Actor>& _selectedObject)
{
    if (ImGui::CollapsingHeader("Mesh Component"))
    {
        ImGui::SeparatorText("Mesh Settings");

        const Ref<ModelComponent> meshComp = _selectedObject->GetModelComponent();
        std::string preview = "Empty";
        if (const Ref<Model> model = meshComp->GetModel()) preview = model->GetAssetName();

        if (ImGui::BeginCombo("##ModelName", preview.c_str()))
        {
            for (auto& [fst, snd] : AssetSubsystem::GetModelStore())
            {
                ImGui::PushID(fst.c_str());
                if (ImGui::Selectable(fst.c_str()))
                {
                    meshComp->SetModel(snd);
                    DE_LOG(LogEditor, Info, "Model on {} set to: {}", _selectedObject->GetName(), snd->GetAssetName())
                }
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        if (ImGui::ArrowButton("##ResetModel", ImGuiDir_Left))
        {
            DE_LOG(LogEditor, Info, "Model on {} reset", _selectedObject->GetName())
            meshComp->SetModel(nullptr);
        }
        ImGui::SetItemTooltip("Reset");
    }
}

void Denix::ActorDetailsWidget::CameraWidget(const Ref<Actor>& _camera)
{
    if (!_camera) return;

    if (const Ref<CameraComponent> camComp = _camera->GetComponent<CameraComponent>())
    {
        glm::vec3& front = _camera->GetTransformComponent()->GetForward();
        glm::vec3& right = _camera->GetTransformComponent()->GetRight();
        glm::vec3& up = _camera->GetTransformComponent()->GetUp();

        ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
        if (ImGui::CollapsingHeader("Camera Component"))
        {
            ImGui::Checkbox("Perspective Projection", &camComp->m_IsPerspective);

            if (camComp->m_IsPerspective)
            {
                ImGui::DragFloat("Fov", &camComp->m_Fov, m_DragSpeed, 1.f, 170.f);
                ImGui::SameLine();
                if (ImGui::ArrowButton("##ResetFov", ImGuiDir_Left)) camComp->m_Fov = 45.f;
                ImGui::SetItemTooltip("Reset");

                ImGui::DragFloat("Near Plane", &camComp->m_NearPlane, m_DragSpeed);
                ImGui::SameLine();
                if (ImGui::ArrowButton("##ResetNear Plane", ImGuiDir_Left)) camComp->m_NearPlane = 0.1f;
                ImGui::SetItemTooltip("Reset");

                ImGui::DragFloat("Far Plane", &camComp->m_FarPlane, m_DragSpeed);
                ImGui::SameLine();
                if (ImGui::ArrowButton("##ResetFar Plane", ImGuiDir_Left)) camComp->m_FarPlane = 1000.f;
                ImGui::SetItemTooltip("Reset");
            }


            ImGui::DragFloat("Rotation Factor", &camComp->m_RotationFactor, m_DragSpeed);
            ImGui::SameLine();
            if (ImGui::ArrowButton("##ResetRotationFactor", ImGuiDir_Left)) camComp->m_RotationFactor = 0.1f;
            ImGui::SetItemTooltip("Reset");

            ImGui::DragFloat("Pitch Rotation Rate", &camComp->m_PitchRotationRate, m_DragSpeed);
            ImGui::SameLine();
            if (ImGui::ArrowButton("##ResetPitchRotationRate", ImGuiDir_Left)) camComp->m_PitchRotationRate = 0.1f;

            ImGui::DragFloat("Yaw Rotation Rate", &camComp->m_YawRotationRate, m_DragSpeed);
            ImGui::SameLine();
            if (ImGui::ArrowButton("##ResetYawRotationRate", ImGuiDir_Left)) camComp->m_YawRotationRate = 0.1f;
            ImGui::SetItemTooltip("Reset");


            if (ImGui::TreeNode("Advance Camera Settings"))
            {
                auto transform = _camera->GetTransformComponent();
                glm::vec3& position = transform->GetPosition();
                glm::vec3& rotation = transform->GetRotation();

                ImGui::DragFloat3("Forward", &front[0], m_DragSpeed);
                ImGui::DragFloat3("Right", &right[0], m_DragSpeed);
                ImGui::DragFloat3("Up", &up[0], m_DragSpeed);

                ImGui::Checkbox("Othro Override", &camComp->m_OrthoOverride);
                if (camComp->m_OrthoOverride)
                {
                    ImGui::DragFloat("Ortho Bottom", &camComp->m_OrthoBottom, m_DragSpeed);
                    ImGui::DragFloat("Ortho Top", &camComp->m_OrthoTop, m_DragSpeed);
                    ImGui::DragFloat("Ortho Left", &camComp->m_OrthoLeft, m_DragSpeed);
                    ImGui::DragFloat("Ortho Right", &camComp->m_OrthoRight, m_DragSpeed);
                }

                ImGui::DragFloat3("Camera Position", &position[0], m_DragSpeed);

                ImGui::SameLine();
                if (ImGui::ArrowButton("##ResetPosition", ImGuiDir_Left)) position = glm::vec3(0.f);
                ImGui::SetItemTooltip("Reset");

                ImGui::DragFloat3("Viewport Rotation", &rotation[0], m_DragSpeed);

                ImGui::SameLine();
                if (ImGui::ArrowButton("##ResetRotation", ImGuiDir_Left)) rotation = glm::vec3(0.f);
                ImGui::SetItemTooltip("Reset");

                ImGui::DragFloat("m_MoveSpeed", &camComp->m_MoveSpeed, m_DragSpeed, 1.f,
                                 10.f);
                ImGui::SameLine();
                if (ImGui::ArrowButton("##ResetMoveSpeed", ImGuiDir_Left)) camComp->m_MoveSpeed;

                ImGui::SetItemTooltip("Reset");

                ImGui::DragFloat("Scoll Wheel Speed", &camComp->m_MouseScrollSpeed,
                                 m_DragSpeed, 0.1f, 10.f);

                ImGui::TreePop();
            }
        }
    }
}
