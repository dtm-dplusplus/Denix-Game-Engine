#include "RendererSubsystem.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Scene/Camera.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Scene/Actor.h"
#include "Denix/Profile/ProfileSubsystem.h"

namespace Denix
{
    void RendererSubsystem::Initialize()
    {
        DE_LOG(LogRender, Warn, "Initializing RendererSubsystem")
        Subsystem::Initialize();
        m_DefaultShader = AssetSubsystem::GetDefaultShader();
        
        DE_LOG(LogRender, Info, "RendererSubsystem Initialized")
    }

    void RendererSubsystem::Deinitialize()
    {
        DE_LOG(LogRender, Trace, "RendererSubsystem Deinitializing")
        DE_LOG(LogRender, Trace, "RendererSubsystem Deinitialized")
    }

    void RendererSubsystem::RenderScene()
    {
        if (!m_Enabled) return;
        
        DE_PROFILE(Render Scene)
        RenderDefaultViewport();
        DE_PROFILE_END(Render Scene)
    }

    void RendererSubsystem::RenderObject(const RenderSubmission& _submission)
    {
        if (!_submission.Mat || !_submission.Model) return;

        // In the future we will batch these submissions, for now we can't assume the same shader and must bind it
        _submission.Mat->m_Shader->Bind();
        
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

    void RendererSubsystem::SubmitCamera(const CameraSubmission& _submission)
    {
        s_Instance->m_CameraSubmission = _submission;
    }

    void RendererSubsystem::RenderDefaultViewport() const
    {
        Ref<Scene> activeScene = m_ActiveScene.lock();
        DE_ASSERT(activeScene, "No Active Scene")
        DE_ASSERT(activeScene->m_ActiveCamera, "No Active Camera in Scene")
        
        auto defaultShader = m_DefaultShader.lock();
        DE_ASSERT(defaultShader, "No Default Shader")

        defaultShader->Bind();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Upload the camera matrices relative to Object
        if (Ref<CameraComponent> camComp = activeScene->m_ActiveCamera->GetComponent<CameraComponent>())
        {
            glUniformMatrix4fv(defaultShader->GetUniform("u_Projection"), 1,
                          GL_FALSE, glm::value_ptr(camComp->m_Projection));

            glUniformMatrix4fv(defaultShader->GetUniform("u_View"), 1,
                               GL_FALSE, glm::value_ptr(camComp->m_View));
        }

        // Render all actors in the scene
        for (const Ref<Actor>& actor : activeScene->m_Actors)
            if (actor->m_RenderComponent->m_IsVisible) 
                RenderObject({actor->m_RenderComponent->m_Material, actor->m_ModelComponent->m_Model, actor->GetTransformComponent()->m_Model});
    }

    void RendererSubsystem::SetActiveScene(const Ref<Scene>& _scene)
    {
        s_Instance->m_ActiveScene = _scene;
    }
}
