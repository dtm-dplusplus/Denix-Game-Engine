
#include "SceneEditorWidget.h"

#include "Denix/UI/UISubsystem.h"
#include "Denix/Scene/Scene.h"

Denix::SceneEditorWidget::SceneEditorWidget(const ObjectInit& _objInit, const WRef<Scene>& _scene): EditorWidget(_objInit)
{
    m_SceneRef = _scene;
}

void Denix::SceneEditorWidget::SceneChangedEvent(const WRef<Scene>& _scene)
{
    m_SceneRef = _scene;
}
