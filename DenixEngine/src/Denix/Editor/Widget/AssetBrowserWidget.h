#pragma once

#include "Denix/Editor/EditorWidget.h"
#include "Denix/Asset/Asset.h"

namespace Denix
{
    class AssetBrowserWidget : public EditorWidget
    {
    public:
        AssetBrowserWidget();

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
    };
}
