#include "GEPScene.h"

#include "Character.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/Actor/Shapes.h"
#include "Denix/Input/InputSubsystem.h"

GEPScene::GEPScene()
{
}

void GEPScene::BeginScene()
{
    Scene::BeginScene();
    m_Character = SpawnActor<Character>(glm::vec3(0.0f, 2.5f, 0.0f));
    //m_CharacterCamera = SpawnActor<Camera>(glm::vec3(0.0f, 2.5f, 0.0f));
    //m_Character->m_Camera = m_CharacterCamera;
    SpawnActor<Plane>(glm::vec3(0.0f), glm::vec3(0.0f), {100.0f, 0.01f, 100.0f});
}

void GEPScene::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::Update(_deltaTime, _waitCounter);
    glm::vec3 movement(0.0f);
    glm::vec3 fwd = m_Character->GetTransformComponent()->GetForward();
    glm::vec3 right = m_Character->GetTransformComponent()->GetRight();
    float m_MoveSpeed = m_Character->m_MoveSpeed;
    
   if (InputSubsystem::IsKeyDown(KeyCode::DEK_W))
{
    //DE_LOG(LogDevProject, Trace, "Forward key is pressed");
    movement += fwd * m_MoveSpeed;
}
if (InputSubsystem::IsKeyDown(KeyCode::DEK_S))
{
    movement -= fwd * m_MoveSpeed;
}
if (InputSubsystem::IsKeyDown(KeyCode::DEK_A))
{
    movement -= right * m_MoveSpeed;
}
if (InputSubsystem::IsKeyDown(KeyCode::DEK_D))
{
    movement += right * m_MoveSpeed;
}

    movement *= _deltaTime;
    
    if (glm::length(movement) > 0.0f)
    {
        DE_LOG(LogDevProject, Trace, "Movement: {} {} {}", movement.x, movement.y, movement.z);
        m_Character->GetPhysicsComponent()->AddImpulse({movement.x, 0.0f, movement.z});
    }
   
}

void GEPScene::DebugUI(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::DebugUI(_deltaTime, _waitCounter);

    ImGui::Begin("GEP Scene");
    ImGui::SeparatorText("Camera");
    auto camComp = m_ActiveCamera->GetCameraComponent();
    ImGui::Checkbox("External Camera Control", &camComp->m_ExternalControl);
    ImGui::Checkbox("Rotation", &camComp->m_EnableRotation);

    static bool MatchCharacter = false;
    ImGui::Checkbox("Match Character", &MatchCharacter);
    static glm::vec3 offset = glm::vec3(0.0f, 1.0f, 0.0f);
    ImGui::DragFloat3("Offset", &offset[0], 0.1f);
    if (MatchCharacter) m_ActiveCamera->GetTransformComponent()->SetPosition(m_Character->GetTransformComponent()->GetPosition() + offset);

    ImGui::DragFloat("Move Speed", &m_Character->m_MoveSpeed, 0.1f, 0.0f, 200.0f);
    ImGui::DragFloat("Jump Force", &m_Character->m_JumpForce, 0.1f, 0.0f, 200.0f);

    static physx::PxVec3 pos= physx::PxVec3(0.0f, 0.5f, 0.0f);
    static physx::PxVec3 direction = physx::PxVec3(0.0f, -1.0f, 0.0f);
    static float maxDistance = 0.5f;

    if (ImGui::Button("Spawn Character"))
    {
        m_Character = SpawnActor<Character>(glm::vec3(2.0f, 2.5f, 0.0f));
    }
    ImGui::DragFloat3("Position", &pos[0], 0.1f);
    ImGui::DragFloat3("Direction", &direction[0], 0.1f);
    ImGui::DragFloat("Max Distance", &maxDistance, 0.1f);
    if (InputSubsystem::IsKeyDown(KeyCode::DEK_SPACE))
    {
        // physx::PxVec3 position = m_Character->GetPhysicsComponent()->m_PxActor->getGlobalPose().p;
        glm::vec3 position = m_Character->GetTransformComponent()->GetPosition();
        //physx::PxVec3 position = pos;

        if (m_Character->GetPhysicsComponent()->m_PxShape->getGeometry().getType() == physx::PxGeometryType::eBOX)
        {
            // Use boxGeometry as needed
            physx::PxGeometryHolder boxGeometry = m_Character->GetPhysicsComponent()->m_PxShape->getGeometry();
            maxDistance = boxGeometry.box().halfExtents.y + 0.1f;
        }

        // Perform the raycast
        physx::PxRaycastBuffer hit;

        physx::PxQueryFilterData filterData(physx::PxQueryFlag::eSTATIC);
        
        bool ishit = m_PxScene->raycast({position.x, position.y, position.z}, {direction.x, direction.y, direction.z}, maxDistance, hit, physx::PxHitFlag::eDEFAULT, filterData);
        if (ishit) m_Character->GetPhysicsComponent()->AddImpulse({0.0f, m_Character->m_JumpForce, 0.0f});
       // DE_LOG(LogDevProject, Trace, "Raycast hit: {}", ishit);
    }
    ImGui::End();
}
