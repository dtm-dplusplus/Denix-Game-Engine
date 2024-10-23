#include "Denix/Scene/Scene.h"

#include "Denix/Resource/Asset.h"

namespace Denix
{
    Scene::Scene(const Ref<Asset>& _sceneAsset): BaseObject(ObjectInitializer(_sceneAsset->GetAssetName())),
                                                 m_SceneName{ _sceneAsset->GetAssetName() },
                                                 m_SceneAsset{ _sceneAsset },
                                                 m_ViewportCamera{ nullptr },
                                                 m_ActiveCamera{ nullptr },
                                                 m_DirLight{ nullptr }
    {
    }

    void Scene::SpawnGameObject(const Ref<GameObject>& _obj)
    {
        if (m_IsOpen)
        {
            _obj->BeginScene();

            if (m_IsPlaying)
                _obj->BeginPlay();
        }

        // Type Checking for lights
        if (typeid(PointLight) == typeid(*_obj))
        {
            if (m_PointLights.size() < MAX_POINT_LIGHTS)
            {
                m_PointLights.push_back(CastRef<PointLight>(_obj));
            }
            else
            {
                DE_LOG(LogScene, Warn, "Max Point Lights Reached")
            }
        }
        else if (typeid(SpotLight) == typeid(*_obj))
        {
            if (m_SpotLights.size() < MAX_SPOT_LIGHTS)
            {
                m_SpotLights.push_back(CastRef<SpotLight>(_obj));
            }
            else
            {
                DE_LOG(LogScene, Warn, "Max Spot Lights Reached")
            }
        }
        else if (typeid(DirectionalLight) == typeid(*_obj))
        {
            // Check if the scene already has a directional light
            if (m_DirLight)
            {
                DE_LOG(LogEditor, Warn, "Scene already has a directional light")
            }
            m_DirLight = CastRef<DirectionalLight>(_obj);
        }
		
        m_SceneObjects.push_back(std::move(_obj));
    }
}
