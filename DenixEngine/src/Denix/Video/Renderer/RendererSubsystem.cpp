#include "RendererSubsystem.h"

#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/Camera.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Scene/Actor.h"
#include "Denix/Physics/Collider.h"
#include "Denix/Profile/ProfileSubsystem.h"

namespace Denix
{
    RendererSubsystem* RendererSubsystem::s_RendererSubSystem{nullptr};

    void RendererSubsystem::Initialize()
    {
        Subsystem::Initialize();
        DE_LOG(LogRenderer, Warn, "Initializing RendererSubsystem")
        m_DefaultShader = ResourceSubsystem::GetShader("DefaultShader");
        DE_LOG(LogRenderer, Info, "RendererSubsystem Initialized")
    }

    void RendererSubsystem::RenderScene()
    {
        DE_PROFILE(Render Scene)

        if (!s_RendererSubSystem->m_Enabled)
        {
            DE_PROFILE_END(Render Scene)
            return;
        }

        RenderDefaultViewport();

        DE_PROFILE_END(Render Scene)
    }

    void RendererSubsystem::RenderDefaultViewport() const
    {
        if (!s_RendererSubSystem->m_ActiveScene->m_ActiveCamera)
        {
            DE_LOG(LogRender, Error, "No Active Camera in Scene")
            return;
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        m_DefaultShader->Bind();

        // Upload the camera matrices relative to Object
        glUniformMatrix4fv(m_DefaultShader->GetUniform("u_Projection"), 1,
                           GL_FALSE, glm::value_ptr(
                               s_RendererSubSystem->m_ActiveScene->m_ActiveCamera->m_Projection));

        glUniformMatrix4fv(m_DefaultShader->GetUniform("u_View"), 1,
                           GL_FALSE, glm::value_ptr(
                               s_RendererSubSystem->m_ActiveScene->m_ActiveCamera->m_View));

        glUniform3f(m_DefaultShader->GetUniform("u_CameraPosition"), 
            s_RendererSubSystem->m_ActiveScene->m_ActiveCamera->m_TransformComponent->m_Position.x,
            s_RendererSubSystem->m_ActiveScene->m_ActiveCamera->m_TransformComponent->m_Position.y,
            s_RendererSubSystem->m_ActiveScene->m_ActiveCamera->m_TransformComponent->m_Position.z);

        
        for (const Ref<Actor>& actor : s_RendererSubSystem->m_ActiveScene->m_Actors)
        {
            if (!actor->m_RenderComponent->IsVisible() || !actor->m_RenderComponent->m_Material || !actor->m_MeshComponent->m_Model) continue;

            // Base color/texture specific settings
            glUniform1i(actor->m_RenderComponent->m_Shader->GetUniform("u_Material.IsBaseTexture"),
                        actor->m_RenderComponent->m_Material->m_IsBaseTexture);

            if (actor->m_RenderComponent->m_Material->m_IsBaseTexture)
            {
                actor->m_RenderComponent->m_Material->m_BaseTexture->Bind();

                // Texture Settings need to move to the material/texture
                GLenum target = actor->m_RenderComponent->m_Material->m_BaseTexture->m_Target;
                glTexParameteri(target, GL_TEXTURE_WRAP_S, actor->m_RenderComponent->m_TextureSettings.WrapMode);
                glTexParameteri(target, GL_TEXTURE_WRAP_T, actor->m_RenderComponent->m_TextureSettings.WrapMode);
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, actor->m_RenderComponent->m_TextureSettings.FilterMode);
                glTexParameteri(target, GL_TEXTURE_MAG_FILTER, actor->m_RenderComponent->m_TextureSettings.FilterMode);
            }
            else
            {
                glUniform3f(actor->m_RenderComponent->m_Shader->GetUniform("u_Material.BaseColor"),
                            actor->m_RenderComponent->m_Material->m_BaseColor.r, actor->m_RenderComponent->m_Material->m_BaseColor.g,
                            actor->m_RenderComponent->m_Material->m_BaseColor.b);
            }

            
            // Upload the model matrix
            glUniformMatrix4fv(actor->m_RenderComponent->m_Shader->GetUniform("u_Model"), 1,
                               GL_FALSE, glm::value_ptr(actor->m_TransformComponent->m_Model));

            // Draw Call
            for (unsigned int i = 0; i < actor->m_MeshComponent->m_Model->m_Meshes.size(); i++)
            {
                if (actor->m_MeshComponent->m_Model->m_Meshes[i]->m_VAO && actor->m_MeshComponent->m_Model->m_Meshes[i]->m_IBO)
                {
                    actor->m_MeshComponent->m_Model->m_Meshes[i]->m_VAO->Bind();
                    actor->m_MeshComponent->m_Model->m_Meshes[i]->m_IBO->Bind();
                    glDrawElements(GL_TRIANGLES, actor->m_MeshComponent->m_Model->m_Meshes[i]->m_IBO->m_IndexCount,
                                   GL_UNSIGNED_INT, 0);
                }
            }
        }
    }

    void RendererSubsystem::SetActiveScene(const Ref<Scene>& _scene)
    {
        s_RendererSubSystem->m_ActiveScene = _scene;
    }
}
