#include "Viewport.h"

#include "Denix/Resource/ResourceSubsystem.h"

Denix::Viewport::Viewport(const int _width, const int _height)
{
    m_Width = _width;
    m_Height = _height;
    m_FrameBuffer = MakeRef<FrameBuffer>(_width, _height);
    m_Mesh = MakeRef<Mesh>();
    m_Mesh->CreateViewportMesh();
}

void Denix::Viewport::DrawViewport() const
{
    m_Mesh->GetVertexArray()->Bind();
    m_Mesh->GetIndexBuffer()->Bind();
    m_Shader->Bind();
    glBindTexture(GL_TEXTURE_2D, m_FrameBuffer->m_TexID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
