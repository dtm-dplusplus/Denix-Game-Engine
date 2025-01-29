#pragma once

#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    class Scene;

    class SceneEditorWidget : public EditorWidget
    {
    public:
        SceneEditorWidget(const ObjectInit& _objInit, const WRef<Scene>& _scene);

        virtual void SceneChangedEvent(const WRef<Scene>& _scene);

    protected:
        WRef<Scene> m_SceneRef;
    };
}
