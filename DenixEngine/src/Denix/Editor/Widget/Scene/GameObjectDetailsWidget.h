#pragma once

#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    class RenderComponent;
    class GameObject;
    class Material;
    class ShaderEditor;
    
    class GameObjectDetailsWidget: public EditorWidget
    {
    public:
        GameObjectDetailsWidget();

        void Update(float _deltaTime) override;
        //void Begin() override;
        //void End() override;

        void LightWidget(const Ref<GameObject>& _selectedObject) const;
        void PhysicsWidget(const Ref<GameObject>& _selectedObject) const;
        void CollisionWidget(const Ref<GameObject>& _selectedObject) const;

        void RenderWidget(const Ref<GameObject>& _selectedObject);
        void MaterialWidget(const Ref<GameObject>& _selectedObject);
        void MaterialSelectionWidget(Ref<RenderComponent>& _rendComp);
        void TextureSelectionWidget(const Ref<Material>& _material);
        void ShaderSelectionWidget(Ref<Material>& _material);
        void MeshWidget(const Ref<GameObject>& _selectedObject);
        void TransformWidget(const Ref<GameObject>& _object) const;
        void CameraWidget(const Ref<GameObject>& _camera) const;
        
        WRef<GameObject> m_GameObjectRef;
        Ref<ShaderEditor> m_ShaderEditor;
    };
}

