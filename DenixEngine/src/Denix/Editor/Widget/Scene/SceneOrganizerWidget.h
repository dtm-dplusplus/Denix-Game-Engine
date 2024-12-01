#pragma once

#include "Denix/Editor/Widget/SceneEditorWidget.h"

namespace Denix
{
    class Scene;
    class Actor;
    class AddActorWidget;
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
        Ref<AddActorWidget> m_AddActorWidget;
        Ref<SceneSettingsWidget> m_SceneSettingsWidget;
    };
}
