#pragma once

#include "Denix/Editor/EditorWidget.h"
#include "Denix/Editor/Widget/SceneEditorWidget.h"

namespace Denix
{
    class SceneSettingsWidget: public SceneEditorWidget
    {
    public:
        SceneSettingsWidget(const WRef<Scene>& _scene);

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
    };
}
