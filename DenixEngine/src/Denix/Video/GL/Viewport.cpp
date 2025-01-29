#include "Viewport.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Video/WindowSubsystem.h"

Denix::Viewport::Viewport():
    m_Width(WindowSubsystem::GetWindowWidth()), m_Height(WindowSubsystem::GetWindowHeight()),
    m_FrameBuffer(MakeRef<FrameBuffer>(m_Width, m_Height)), m_Shader(AssetSubsystem::GetFrameBufferShader()),
    m_Mesh(MakeRef<Mesh>())
{
    m_Mesh->CreateViewportMesh();
}

Denix::Viewport::Viewport(const int _width, const int _height):
    m_Width(_width), m_Height(_height),
    m_FrameBuffer(MakeRef<FrameBuffer>(_width, _height)), m_Shader(AssetSubsystem::GetFrameBufferShader()),
    m_Mesh(MakeRef<Mesh>())
{
    m_Mesh->CreateViewportMesh();
}

void Denix::Viewport::DrawViewport() const
{
    m_Mesh->GetVertexArray()->Bind();
    m_Mesh->GetIndexBuffer()->Bind();
    m_Shader->Bind();
    glBindTexture(GL_TEXTURE_2D, m_FrameBuffer->m_TexID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
