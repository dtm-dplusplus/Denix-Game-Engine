#include "YamlScene.h"

#include "imgui.h"
#include "Denix/Scene/Object/Shapes/Shapes.h"

Denix::YamlScene::YamlScene() : Scene({"YamlScene"})
{

}

bool Denix::YamlScene::Load()
{
    Scene::Load();

    Ref<Plane> plane = MakeRef<Plane>(ObjectInitializer("Floor"));
    plane->GetTransformComponent()->GetScale() = glm::vec3(10.0f);
    m_SceneObjects.push_back(plane);

    return true;
}

void Denix::YamlScene::Update(float _deltaTime)
{
    Scene::Update(_deltaTime);

    if (ImGui::Button("Create Yaml Object"))
    {

    }
}
