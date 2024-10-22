#pragma once
#include "Denix/Resource/Asset.h"
#include "Denix/Scene/Scene.h"

using namespace Denix;

/*template <class T, class PropertyType> 
    static	void RegisterProperty(	const char* szName, typename CProperty<T, PropertyType>::GetterType Getter, 
                                    typename CProperty<T, PropertyType>::SetterType Setter )
{
    CProperty<T, PropertyType>* pProperty = new CProperty<T, PropertyType>( szName, Getter, Setter );
    T::GetClassRTTI()->GetProperties()->push_back( pProperty );
    CPropertySystem::GetProperties()->push_back( pProperty );
}*/



class DevScene : public Scene
{
public:
    DevScene();
    DevScene(const Ref<Asset>& _sceneAsset);
    ~DevScene() override = default;

    void Update(float _deltaTime) override;

    bool Load() override;
    void Unload() override;

    Ref<DirectionalLight> m_DirectionalLight;
    bool ShowEngineContent;
};