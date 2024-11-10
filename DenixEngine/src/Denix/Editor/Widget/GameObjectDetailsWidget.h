#pragma once

#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    class GameObject;

    class GameObjectDetailsWidget: public EditorWidget
    {
    public:
        GameObjectDetailsWidget();

        //void Update(float _deltaTime) override;
        //void Begin() override;
        //void End() override;

        WRef<GameObject> m_GameObjectRef;
    };
}

