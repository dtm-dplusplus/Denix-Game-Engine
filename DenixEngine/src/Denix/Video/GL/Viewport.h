#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBuffer.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Denix/Scene/MeshData.h"

namespace Denix
{
    class Viewport
    {
    public:
        Viewport(const int _width, const int _height)
        {
            m_Width = _width;
            m_Height = _height;
            m_FrameBuffer = MakeRef<FrameBuffer>(_width, _height);
            m_Mesh = MakeRef<Mesh>();
            m_Mesh->CreateViewportMesh();
        }

        ~Viewport() = default;


        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

        Ref<FrameBuffer> GetFrameBuffer() const { return m_FrameBuffer; }

        int m_Width, m_Height;
        Ref<FrameBuffer> m_FrameBuffer;
        Ref<Shader> m_Shader;
        Ref<Mesh> m_Mesh;
    };
}

