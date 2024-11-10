#pragma once

#include "Denix/Core.h"


namespace Denix
{
    class Shader;

    
    class ShaderEditor
    {
    public:
        ShaderEditor(const Ref<Shader>& _shader);
        ~ShaderEditor() = default;
        Ref<Shader> ShaderObject;
        
        bool IsOpen;
        void Update();
    };
}
