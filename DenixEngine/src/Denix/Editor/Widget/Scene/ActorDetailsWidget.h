#pragma once

#include "Denix/Editor/EditorWidget.h"
#include "Denix/Scene/Scene.h"

namespace Denix
{
    class RenderComponent;
    class Actor;
    class Material;
    
    class ActorDetailsWidget: public EditorWidget
    {
    public:
        ActorDetailsWidget();

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        void PhysicsWidget(const Ref<Actor>& _selectedObject) const;

        void RenderWidget(const Ref<Actor>& _selectedObject);
        void MaterialWidget(const Ref<Actor>& _selectedObject);
        void TextureSelectionWidget(const Ref<Material>& _material);
        void ShaderSelectionWidget(Ref<Material>& _material);
        void MeshWidget(const Ref<Actor>& _selectedObject);
        void TransformWidget(const Ref<Actor>& _actor) const;
        static void CameraWidget(const Ref<Actor>& _camera);

        WRef<Actor> m_ActorRef;
    };
}

