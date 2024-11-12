#pragma once

#include "Denix/Editor/Widget/SceneEditorWidget.h"

namespace Denix
{
    class Scene;
    class Actor;
    class AddGameObjectWidget;
    class SceneSettingsWidget;

    class SceneOrganizerWidget: public SceneEditorWidget
    {
    public:
        SceneOrganizerWidget(const WRef<Scene>& _scene);

        void Update(float _deltaTime) override;
        void SceneChangedEvent(const WRef<Scene>& _scene) override;
        void ResetSelection();
        void SetSelection(int _index);
        Ref<Actor> GetSelectedObject() const;

        bool ValidateSelection() const;
        int m_SelectionIndex;

    private:
        Ref<AddGameObjectWidget> m_AddGameObjectWidget;
        Ref<SceneSettingsWidget> m_SceneSettingsWidget;
    };
}
