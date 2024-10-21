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

    void Scene::SpawnSceneObject(const Ref<GameObject>& _object)
    {
        // An object can be spawned as long as the level is loaded. It doesn't have to be open
        /*if (!m_IsLoaded) 
        {
            DE_LOG(LogScene, Error, "Scene is not loaded. Failed To Spawn Object: {}", _object->GetName())
            return;
        }*/

        if (m_IsOpen)
        {
            _object->BeginScene();

            if (m_IsPlaying)
                _object->BeginPlay();
        }

        // Type Checking for lights
        if (typeid(PointLight) == typeid(*_object))
        {
            if (m_PointLights.size() < MAX_POINT_LIGHTS)
            {
                m_PointLights.push_back((std::dynamic_pointer_cast<PointLight>(_object)));
            }
            else
            {
                DE_LOG(LogScene, Warn, "Max Point Lights Reached")
            }
        }
        else if (typeid(SpotLight) == typeid(*_object))
        {
            if (m_SpotLights.size() < MAX_SPOT_LIGHTS)
            {
                m_SpotLights.push_back(std::dynamic_pointer_cast<SpotLight>(_object));
            }
            else
            {
                DE_LOG(LogScene, Warn, "Max Spot Lights Reached")
            }
        }
        else if (typeid(DirectionalLight) == typeid(*_object))
        {
            // Check if the scene already has a directional light
            if (m_DirLight)
            {
                DE_LOG(LogEditor, Warn, "Scene already has a directional light")
                return;
            }
            m_DirLight = std::dynamic_pointer_cast<DirectionalLight>(_object);
        }
		
        m_SceneObjects.push_back(std::move(_object));
    }
}
