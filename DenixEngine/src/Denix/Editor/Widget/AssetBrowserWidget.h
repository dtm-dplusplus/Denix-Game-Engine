#pragma once

#include "Denix/Editor/EditorWidget.h"
#include "Denix/Resource/Asset.h"

namespace Denix
{
    class AssetBrowserWidget: public EditorWidget
    {
    public:
        AssetBrowserWidget();

        void Update(float _deltaTime) override;
    };
}

