#pragma once

#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    class Scene;
    class GameObject;
    class AddGameObjectWidget;
    
    class SceneOrganizerWidget: public EditorWidget
    {
    public:
        SceneOrganizerWidget(const WRef<Scene>& _scene);

        void Update(float _deltaTime) override;

        void SetObjectSelection(int _index);
        Ref<GameObject> GetSelectedObject() const;
        WRef<Scene> m_SceneRef;

        bool ValidateSelectedObject() const;
        int m_SelectedObjectIndex;

    private:
        Ref<AddGameObjectWidget> m_AddGameObjectWidget;
    };
}
