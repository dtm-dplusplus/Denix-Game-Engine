#include "PhysicsScene.h"

#include "imgui.h"
#include "Denix/Physics/PhysicsSubsystem.h"

PhysicsScene::PhysicsScene()
{
   
}

PhysicsScene::PhysicsScene(const ObjectInit& _objInit): Scene(_objInit)
{
    
}

void PhysicsScene::BeginScene()
{
    Scene::BeginScene();

    Spheres = GetActorsOfClass<Sphere>();
}

void PhysicsScene::BeginPlay()
{
    Scene::BeginPlay();

    for(const auto sphere: Spheres)
    {
        sphere->GetPhysicsComponent()->AddImpulse(StartImpulse);
    }
}

void PhysicsScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::Begin("Physics Scene");
    ImGui::DragFloat3("Start Impulse", &StartImpulse[0], 0.1f);
    ImGui::Text("collisons: %d", PhysicsSubsystem::Get()->GetCollisionEvents().size());
    ImGui::Text("Physics Components: %d", PhysicsSubsystem::Get()->GetPhysicsComponents().size());
    ImGui::End();
}
