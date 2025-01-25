#pragma once

#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    class Scene;
    class Actor;

    class AddActorWidget: public EditorWidget
    {
    public:
        AddActorWidget(const WRef<Scene>& _scene);

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        WRef<Scene> m_SceneRef;
        bool m_CreatedActor;
    };
}

