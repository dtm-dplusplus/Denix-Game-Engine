#pragma once

#include "Denix/Editor/EditorWidget.h"
#include "Denix/Scene/Scene.h"

namespace Denix
{
    class RenderComponent;
    class Actor;
    class Material;
    class ShaderEditor;
    
    class ActorDetailsWidget: public EditorWidget
    {
    public:
        ActorDetailsWidget();

        void Update(float _deltaTime) override;
        //void Begin() override;
        //void End() override;

        void PhysicsWidget(const Ref<Actor>& _selectedObject) const;
        void CollisionWidget(const Ref<Actor>& _selectedObject) const;

        void RenderWidget(const Ref<Actor>& _selectedObject);
        void MaterialWidget(const Ref<Actor>& _selectedObject);
        void MaterialSelectionWidget(Ref<RenderComponent>& _rendComp);
        void TextureSelectionWidget(const Ref<Material>& _material);
        void ShaderSelectionWidget(Ref<Material>& _material);
        void MeshWidget(const Ref<Actor>& _selectedObject);
        void TransformWidget(const Ref<Actor>& _object) const;
        void CameraWidget(const Ref<Actor>& _camera) const;

        WRef<Actor> m_ActorRef;
        Ref<ShaderEditor> m_ShaderEditor;
    };
}

