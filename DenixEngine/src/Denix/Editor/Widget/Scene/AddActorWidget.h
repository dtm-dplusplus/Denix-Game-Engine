#pragma once

#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    class Scene;
    class Actor;

    class AddActorWidget
    {
    public:
        AddActorWidget(const WRef<Scene>& _scene);

        void Update(float _deltaTime);

        WRef<Scene> m_SceneRef;
        bool m_CreatedActor;
    };
}

