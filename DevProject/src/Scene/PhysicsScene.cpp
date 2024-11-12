#include "PhysicsScene.h"

#include "imgui.h"

PhysicsScene::PhysicsScene()
{
   
}

PhysicsScene::PhysicsScene(const ObjectInit& _objInit): Scene(_objInit)
{
    
}

void PhysicsScene::BeginScene()
{
    Scene::BeginScene();

    /*WRef<Cube> floor = SpawnGameObject<Cube>();
    floor.lock()->GetTransformComponent()->SetScale(100.0f, 1.0f, 100.0f);
    */
    // Spawn physics spheres
    for (int i = 0; i < 10; i++)
    {
        Ref<Sphere> sphere = SpawnGameObject<Sphere>();
        sphere->GetTransformComponent()->SetPosition(i * 3.0f, 10.0f, 0.0f);
        sphere->GetPhysicsComponent()->SimulatePhysics() = true;
       Spheres.push_back(sphere);
    }
}

void PhysicsScene::BeginPlay()
{
    Scene::BeginPlay();

    for(auto sphere: Spheres)
    {
        sphere->GetPhysicsComponent()->AddImpulse(StartImpulse);
    }
}

void PhysicsScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::Begin("Physics Scene");
    ImGui::DragFloat3("Start Impulse", &StartImpulse[0], 0.1f);
    ImGui::End();
}
