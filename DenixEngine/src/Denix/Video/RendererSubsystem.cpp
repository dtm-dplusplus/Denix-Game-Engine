#include "RendererSubsystem.h"

#include "Denix/Profile/ProfileSubsystem.h"

namespace Denix
{
    void RendererSubsystem::Initialize()
    {
        DE_LOG(LogRender, Warn, "Initializing RendererSubsystem")
        Subsystem::Initialize();
        DE_LOG(LogRender, Info, "RendererSubsystem Initialized")
    }

    void RendererSubsystem::Deinitialize()
    {
        DE_LOG(LogRender, Trace, "RendererSubsystem Deinitializing")
        Subsystem::Deinitialize();
        DE_LOG(LogRender, Trace, "RendererSubsystem Deinitialized")
    }

    void RendererSubsystem::SubmitCamera(const CameraSubmission& _submission)
    {
        s_Instance->m_CameraSubmission = _submission;
    }
    
    void RendererSubsystem::RenderObject(const RenderSubmission& _submission)
    {
        if (!_submission.Mat || !_submission.Model) return;

        // In the future we will batch these submissions, for now we can't assume the same shader and must bind it
        _submission.Mat->m_Shader->Bind();

        // Camer matrices
        glUniformMatrix4fv( _submission.Mat->m_Shader->GetUniform("u_Projection"), 1,
                         GL_FALSE, glm::value_ptr(s_Instance->m_CameraSubmission.Projection));

        glUniformMatrix4fv( _submission.Mat->m_Shader->GetUniform("u_View"), 1,
                           GL_FALSE, glm::value_ptr(s_Instance->m_CameraSubmission.View));
        
        // Base color/texture specific settings
        glUniform1i(_submission.Mat->m_Shader->GetUniform("u_Material.IsBaseTexture"),
                    _submission.Mat->m_IsBaseTexture);

        if (_submission.Mat->m_IsBaseTexture)
        {
            _submission.Mat->m_BaseTexture->Bind();

            GLenum target = _submission.Mat->m_BaseTexture->m_Target;
            glTexParameteri(target, GL_TEXTURE_WRAP_S, _submission.Mat->m_TextureSettings.WrapMode);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, _submission.Mat->m_TextureSettings.WrapMode);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, _submission.Mat->m_TextureSettings.FilterMode);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER,_submission.Mat->m_TextureSettings.FilterMode);
        }
        else
        {
            glUniform3f(_submission.Mat->m_Shader->GetUniform("u_Material.BaseColor"),
                        _submission.Mat->m_BaseColor.r,
                        _submission.Mat->m_BaseColor.g,
                        _submission.Mat->m_BaseColor.b);
        }

        // Upload the model matrix
        glUniformMatrix4fv(_submission.Mat->m_Shader->GetUniform("u_Model"), 1,
                           GL_FALSE, glm::value_ptr(_submission.TransformModel));

        // Draw Call
        for (const auto& mesh : _submission.Model->m_Meshes)
        {
            if (mesh->m_VAO && mesh->m_IBO)
            {
                mesh->m_VAO->Bind();
                mesh->m_IBO->Bind();
                glDrawElements(GL_TRIANGLES, mesh->m_IBO->m_IndexCount,GL_UNSIGNED_INT, 0);
            }
        }
    }
}
