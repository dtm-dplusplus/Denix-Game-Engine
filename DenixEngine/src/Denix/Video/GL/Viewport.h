#pragma once

#include "FrameBuffer.h"
#include "Shader.h"
#include "Mesh.h"

namespace Denix
{
    class Viewport: public Object
    {
    public:

        
        /**
         *  @brief Constructor for the Viewport class. Will create a viewport with the size of the window
         * @param _objInit 
         */
        Viewport(const ObjectInit& _objInit = {"Viewport"});

        
        /**
         *  @brief Constructor for the Viewport class. Will create a viewport with the specified size
         * @param _width  
         * @param _height 
         * @param _objInit 
         */
        Viewport(int _width, int _height, const ObjectInit& _objInit = {"Viewport"});

        ~Viewport() override = default;

        void DrawViewport() const;
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

        Ref<FrameBuffer> GetFrameBuffer() const { return m_FrameBuffer; }

        int m_Width, m_Height;
        Ref<FrameBuffer> m_FrameBuffer;
        Ref<Shader> m_Shader;
        Ref<Mesh> m_Mesh;
    };
}

