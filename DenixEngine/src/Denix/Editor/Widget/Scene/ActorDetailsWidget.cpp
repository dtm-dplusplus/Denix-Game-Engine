#include "ActorDetailsWidget.h"

#include "imgui.h"
#include "Denix/UI/UISubsystem.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/Scene/Camera.h"
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
        CollisionWidget(actorRef);
        RenderWidget(actorRef);
        MeshWidget(actorRef);
    }

    ImGui::End();
}

void Denix::ActorDetailsWidget::TransformWidget(const Ref<Actor>& _object) const
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
    if (ImGui::CollapsingHeader("Transform Component"))
    {
        const Ref<TransformComponent> transform = _object->GetTransformComponent();

        ImGui::DragFloat3("Position", &transform->GetPosition()[0], m_DragSpeed);
        ImGui::SameLine();
        if (ImGui::ArrowButton("##ResetPosition", ImGuiDir_Left)) transform->SetPosition(glm::vec3(0.f));
        ImGui::SetItemTooltip("Reset");

        ImGui::DragFloat3("Rotation", &transform->GetRotation()[0], m_DragSpeed);
        ImGui::SameLine();
        if (ImGui::ArrowButton("##ResetRotation", ImGuiDir_Left)) transform->SetRotation(glm::vec3(0.f));
        ImGui::SetItemTooltip("Reset");

        ImGui::DragFloat3("Scale", &transform->GetScale()[0], m_DragSpeed, FLT_MIN);
        ImGui::SameLine();
        if (ImGui::ArrowButton("##ResetScale", ImGuiDir_Left)) transform->SetScale(glm::vec3(1.f));
        ImGui::SetItemTooltip("Reset");

        ImGui::Spacing();
        ImGui::SeparatorText("Moveability");
        if (ImGui::Combo("Moveability", &transform->GetMoveabilityI(), "Static\0Dynamic\0\0"))
        {
            transform->SetMoveability(static_cast<Moveability>(transform->GetMoveabilityI()));
        }
    }
}

void Denix::ActorDetailsWidget::PhysicsWidget(const Ref<Actor>& _selectedObject) const
{
    if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const Ref<PhysicsComponent> comp = _selectedObject->GetPhysicsComponent();

        // Physics Simulation
        if (ImGui::Checkbox("Simulate Physics", &comp->SimulatePhysics()))  comp->m_AttributeFlags |= PHYSICS_SIMULATE;

        if (ImGui::Checkbox("Simulate Collision", &comp->CollisionDetectionEnabled())) comp->m_AttributeFlags |= PHYSICS_COLLISION;
        
        // Mass
        if (ImGui::DragFloat("Mass", &comp->m_Mass, m_DragSpeed, 0.0f, FLT_MAX)) comp->m_AttributeFlags |= PHYSICS_MASS;

        // Linear Drag
        if (ImGui::DragFloat("Linear Drag", &comp->m_LinearDrag, m_DragSpeed, 0.0f, FLT_MAX)) comp->m_AttributeFlags |= PHYSICS_LINEAR_DRAG;

        // Angular Drag
        if (ImGui::DragFloat("Angular Drag", &comp->m_AngularDrag, m_DragSpeed, 0.0f, FLT_MAX)) comp->m_AttributeFlags |= PHYSICS_ANGULAR_DRAG;

        // Elasticity
        if (ImGui::DragFloat("Elasticity", &comp->m_Elasticity, m_DragSpeed, 0.0f, 1.0f)) comp->m_AttributeFlags |= PHYSICS_MATERIAL;

        // Static Friction
        if (ImGui::DragFloat("Static Friction", &comp->m_StaticFriction, m_DragSpeed, 0.0f, 1.0f)) comp->m_AttributeFlags |= PHYSICS_MATERIAL;


        // Dynamic Friction
        if (ImGui::DragFloat("Dynamic Friction", &comp->m_DynamicFriction, m_DragSpeed, 0.0f, 1.0f)) comp->m_AttributeFlags |= PHYSICS_MATERIAL;
        
        if (ImGui::TreeNode("Advanced Settings"))
        {
            // Impulse Response
            ImGui::Checkbox("Impulse Resonses", &comp->GetImpulseEnabled());
            ImGui::TreePop();
        }

        // Viewable Properties
        const glm::vec3 force = comp->GetForce();
        const glm::vec3& vel = comp->GetVelocity();
        const glm::vec3& avel = comp->GetAngularVelocity();
        const glm::vec3& acc = comp->GetAcceleration();

        ImGui::Text("Force				x: %.2f y: %.2f z: %.2f", force.x, force.y, force.z);
        ImGui::Text("Velocity			x: %.2f y: %.2f z: %.2f", vel.x, vel.y, vel.z);
        ImGui::Text("Acceleration		x: %.2f y: %.2f z: %.2f", acc.x, acc.y, acc.z);
        ImGui::Text("Angular Velocity	x: %.2f y: %.2f z: %.2f", avel.x, avel.y, avel.z);
    }
}

void Denix::ActorDetailsWidget::CollisionWidget(const Ref<Actor>& _selectedObject) const
{
    if (ImGui::CollapsingHeader("Collision", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (const Ref<PhysicsComponent> pComp = _selectedObject->GetPhysicsComponent())
        {
            // Collider Type
            static const char* colliderTypes[] = {"None", "Cube", "Sphere"};
            int itemCurrent = pComp->GetCollider() ? ((int)pComp->GetCollider()->GetColliderType() + 1) : 0;
            const char* previewItem = colliderTypes[itemCurrent];
            if (ImGui::BeginCombo("Collider Type", previewItem))
            {
                for (int n = 0; n < IM_ARRAYSIZE(colliderTypes); n++)
                {
                    const bool is_selected = (itemCurrent == n);
                    if (ImGui::Selectable(colliderTypes[n], is_selected))
                    {
                        itemCurrent = n;
                        switch (itemCurrent)
                        {
                        case 0:
                            {
                                pComp->SetCollider(nullptr);
                                DE_LOG(LogEditor, Warn, "Set collider type to none on {}", _selectedObject->GetName())
                            }
                            break;

                        case 1:
                            {
                                pComp->SetCollider(MakeRef<CubeCollider>());
                                DE_LOG(LogEditor, Warn, "Set collider type to cube on {}", _selectedObject->GetName())
                            }
                            break;

                        case 2:
                            {
                                pComp->SetCollider(MakeRef<SphereCollider>());
                                DE_LOG(LogEditor, Warn, "Set collider type to sphere on {}", _selectedObject->GetName())
                            }
                            break;
                        }
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // Collider Visualiser
            /*ImGui::Checkbox("Show Collider", &pComp->IsColliderVisible());*/

            // Collider settings
            /*if (pComp->GetCollider())
            {
                switch (pComp->GetCollider()->GetColliderType())
                {
                case ColliderType::Cube:
                    {
                        if (Ref<CubeCollider> sCol = CastRef<CubeCollider>(pComp->GetCollider()))
                        {
                            // Collider Offset
                            ImGui::DragFloat3("Offset", &sCol->GetOffset()[0], m_DragSpeed, FLT_MIN, FLT_MAX);

                            // Collider Dimensions
                            if (!pComp->CollisionDimensionOverride()) ImGui::BeginDisabled();
                            ImGui::DragFloat3("Dimensions", &sCol->GetDimensions()[0], m_DragSpeed, -FLT_MIN, FLT_MAX);
                            if (!pComp->CollisionDimensionOverride()) ImGui::EndDisabled();
                            ImGui::SameLine();
                            ImGui::Checkbox("## Dimesnion Override", &pComp->CollisionDimensionOverride());

                            // Min Max Debug Info
                            const glm::vec3& min = sCol->GetMin();
                            const glm::vec3& max = sCol->GetMax();
                            ImGui::Text("AABB Min x: %.2f Max x: %.2f", min.x, max.x);
                            ImGui::Text("AABB Min y: %.2f Max y: %.2f", min.y, max.y);
                            ImGui::Text("AABB Min z: %.2f Max z: %.2f", min.z, max.z);
                        }
                    }
                    break;

                case ColliderType::Sphere:
                    {
                        if (Ref<SphereCollider> sCol = CastRef<SphereCollider>(pComp->GetCollider()))
                        {
                            ImGui::DragFloat("Radius", &sCol->GetRadius(), m_DragSpeed, FLT_MIN, FLT_MAX);
                        }
                    }
                    break;
                }
            }*/
        }
    }
}

void Denix::ActorDetailsWidget::RenderWidget(const Ref<Actor>& _selectedObject)
{
    if (ImGui::CollapsingHeader("Rendering", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const Ref<RenderComponent> render = _selectedObject->GetRenderComponent();

        ImGui::Checkbox("Visible", &render->IsVisible());
        MaterialWidget(_selectedObject);
    }
}

void Denix::ActorDetailsWidget::MaterialWidget(const Ref<Actor>& _selectedObject)
{
    Ref<RenderComponent> rendComp = _selectedObject->GetRenderComponent();

    ImGui::SeparatorText("Material");

    // Material Settings
    if (Ref<Material>& mat = rendComp->GetMaterial())
    {
        MaterialSelectionWidget(rendComp);

        // Material Properties
        ImGui::DragFloat("AO", &mat->GetAO(), m_DragSpeed, 0.0f, 1.0f);
        ImGui::DragFloat("Metallic", &mat->GetMetallic(), m_DragSpeed, 0.0f, 1.0f);
        ImGui::DragFloat("Roughness", &mat->GetRoughness(), m_DragSpeed, 0.0f, 1.0f);
        // Color or Texture selectable
        {
            ImGui::Text("Base Color");
            ImGui::ColorEdit3("Base Color", &mat->GetBaseColor()[0]);
            TextureSelectionWidget(mat);
            if (ImGui::Button("Clear Texture"))
            {
                mat->ClearBaseTexture();
            }
        }

        ImGui::DragFloat("Specular Intensity", &mat->GetSpecularIntensity());
        ImGui::DragFloat("Specular Power", &mat->GetSpecularPower());

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

       // if (m_ShaderEditor) m_ShaderEditor->Update();

        //	// Texture Info
        //	ImGui::SeparatorText("Texture Info");
        //	ImGui::Text("Texture ID: %d", texture->GetTextureID());
        //	ImGui::TextWrapped("File Path : % s", texture->GetFileLocation().c_str());
        //	ImGui::Text("Size = %d x %d", texture->GetWidth(), texture->GetHeight());
        //	ImGui::Separator();

        //	// Texture Settings
        //	TextureSettings& texSettings = render->GetTextureSettings();
        //	if (ImGui::Combo("Wrap Mode", &texSettings.WrapValue, "GL_REPEAT\0GL_MIRRORED_REPEAT\0GL_CLAMP_TO_EDGE\0GL_CLAMP_TO_BORDER\0\0"))
        //	{
        //		if (texSettings.WrapValue == 0) texSettings.WrapMode = GL_REPEAT;
        //		else if (texSettings.WrapValue == 1) texSettings.WrapMode = GL_MIRRORED_REPEAT;
        //		else if (texSettings.WrapValue == 2) texSettings.WrapMode = GL_CLAMP_TO_EDGE;
        //		else if (texSettings.WrapValue == 3) texSettings.WrapMode = GL_CLAMP_TO_BORDER;
        //	}

        //	// Texture Filter
        //	if (ImGui::Combo("Filter", &texSettings.FilterValue, "GL_NEAREST\0GL_LINEAR\0GL_NEAREST_MIPMAP_NEAREST\0GL_LINEAR_MIPMAP_NEAREST\0GL_NEAREST_MIPMAP_LINEAR\0GL_LINEAR_MIPMAP_LINEAR\0\0"))
        //	{
        //		if (texSettings.FilterValue == 0) texSettings.FilterMode = GL_NEAREST;
        //		else if (texSettings.FilterValue == 1) texSettings.FilterMode = GL_LINEAR;
        //		else if (texSettings.FilterValue == 2) texSettings.FilterMode = GL_NEAREST_MIPMAP_NEAREST;
        //		else if (texSettings.FilterValue == 3) texSettings.FilterMode = GL_LINEAR_MIPMAP_NEAREST;
        //		else if (texSettings.FilterValue == 4) texSettings.FilterMode = GL_NEAREST_MIPMAP_LINEAR;
        //		else if (texSettings.FilterValue == 5) texSettings.FilterMode = GL_LINEAR_MIPMAP_LINEAR;
        //	}

        //}
    }
}

void Denix::ActorDetailsWidget::MaterialSelectionWidget(Ref<RenderComponent>& _rendComp)
{
    if (ImGui::BeginCombo("##MaterialName", _rendComp->GetMaterial()->GetName().c_str(),
                          ImGuiComboFlags_WidthFitPreview))
    {
        for (auto& [fst, snd] : AssetSubsystem::GetMaterialStore())
        {
            ImGui::PushID(fst.c_str());
            if (ImGui::Selectable(fst.c_str(), false,
                                  ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap,
                                  ImVec2(250, 100)))
            {
                _rendComp->SetMaterial(snd);
            }
            ImGui::PopID();
        }
        ImGui::EndCombo();
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
            ImGui::Image((void*)(intptr_t)id, ImVec2(100, 100));
            ImGui::SameLine();
        }
    }

    // Texture Selection
    if (ImGui::BeginCombo("##TextureSelection", preview.c_str(), ImGuiComboFlags_WidthFitPreview))
    {
        for (auto& snd : AssetSubsystem::GetTextureStore() | std::views::values)
        {
            ImGui::PushID(snd->GetAssetFileName().c_str());
            ImGui::Image((void*)(intptr_t)snd->GetTextureID(), ImVec2(100, 100));
            ImGui::SameLine();
            if (ImGui::Selectable(snd->GetAssetFileName().c_str(), false,
                                  ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap,
                                  ImVec2(250, 100)))
            {
                _material->SetBaseTexture(snd);
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

        const Ref<ModelComponent> meshComp = _selectedObject->GetMeshComponent();
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
