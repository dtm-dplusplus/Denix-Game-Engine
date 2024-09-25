#pragma once
#include "Denix/Resource/Asset.h"
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
    void Serialize(YAML::Emitter& _out) ;
    void Deserialize(YAML::Node& _in) ;

    std::vector<Ref<Asset> > m_Assets;
    Ref<Asset> m_SceneAsset;
}
;
