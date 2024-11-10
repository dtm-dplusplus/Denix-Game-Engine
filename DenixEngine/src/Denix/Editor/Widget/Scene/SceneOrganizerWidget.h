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

        void ResetSelection();
        void SetSelection(int _index);
        Ref<GameObject> GetSelectedObject() const;
        WRef<Scene> m_SceneRef;

        bool ValidateSelection() const;
        int m_SelectionIndex;

    private:
        Ref<AddGameObjectWidget> m_AddGameObjectWidget;
    };
}
