#pragma once
#include "Denix.h"
#include "Denix/Scene/Scene.h"

#include "yaml-cpp/yaml.h"

using namespace Denix;

class DevScene : public Scene
{
public:
    DevScene();
    ~DevScene() override = default;

    void Update(float _deltaTime) override;

    bool Load() override;
    void Unload() override;
    void Serialize(YAML::Emitter& _out);
    void Deserialize(const YAML::Node& _in);

    YAML::Emitter Emitter;
    YAML::Node Node;
    Ref<GameObject> YamlObj;
};
