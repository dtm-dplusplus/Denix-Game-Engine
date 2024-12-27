#pragma once

#include "FrameBuffer.h"
#include "Shader.h"
#include "Mesh.h"

namespace Denix
{
    class Viewport
    {
    public:

        
        /**
         *  @brief Constructor for the Viewport class. Will create a viewport with the size of the window
         */
        Viewport();

        /**
         *  @brief Constructor for the Viewport class. Will create a viewport with the specified size
         * @param _width  
         * @param _height 
         */
        Viewport(int _width, int _height);

        ~Viewport() = default;

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

