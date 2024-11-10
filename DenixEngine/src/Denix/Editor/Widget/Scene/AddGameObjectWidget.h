#pragma once

#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    class Scene;
    class GameObject;

    class AddGameObjectWidget
    {
    public:
        AddGameObjectWidget(const WRef<Scene>& _scene);

        void Update(float _deltaTime);

        WRef<Scene> m_SceneRef;
        bool m_CreatedGameObject;
    };
}

