#pragma once

#include "Denix/Editor/EditorWidget.h"


namespace Denix
{
    class Shader;

    
    class ShaderEditor: public EditorWidget
    {
    public:
        ShaderEditor(const Ref<Shader>& _shader);
        ~ShaderEditor() override = default;
        WRef<Shader> m_ShaderRef;
        
        void Update(float _deltaTime)  override;
    };
}
