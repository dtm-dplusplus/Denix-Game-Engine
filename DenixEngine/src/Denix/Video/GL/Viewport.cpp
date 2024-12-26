#include "Viewport.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Video/Window/WindowSubsystem.h"

Denix::Viewport::Viewport(const ObjectInit& _objInit): Object(_objInit)
{
    const glm::vec2 size = WindowSubsystem::GetWindowSize();
    m_Width = size.x;
    m_Height = size.y;
    m_FrameBuffer = MakeRef<FrameBuffer>(m_Width, m_Height);
    m_Mesh = MakeRef<Mesh>();
    m_Mesh->CreateViewportMesh();
    m_Shader = AssetSubsystem::GetFrameBufferShader();
}

Denix::Viewport::Viewport(const int _width, const int _height, const ObjectInit& _objInit) : Object(_objInit)
{
    m_Width = _width;
    m_Height = _height;
    m_FrameBuffer = MakeRef<FrameBuffer>(_width, _height);
    m_Mesh = MakeRef<Mesh>();
    m_Mesh->CreateViewportMesh();
    m_Shader = AssetSubsystem::GetShader("FBShader");
}

void Denix::Viewport::DrawViewport() const
{
    m_Mesh->GetVertexArray()->Bind();
    m_Mesh->GetIndexBuffer()->Bind();
    m_Shader->Bind();
    glBindTexture(GL_TEXTURE_2D, m_FrameBuffer->m_TexID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}