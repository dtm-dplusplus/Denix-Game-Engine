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

        /*std::vector<ShaderSource> defaultShaders;
        defaultShaders.emplace_back(R"(Content/Engine/shaders/UI/UIVertex.glsl)");
        defaultShaders.emplace_back(R"(Content/Engine/shaders/UI/UIFragment.glsl)");
		
       m_UIShader= AssetSubsystem::LoadShader(defaultShaders, "UIShader");*/
        //DE_ASSERT(m_UIShader, "UI Shader not loaded")
        
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
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //m_UIShader->Bind();
        // Draw Call
        /*for (unsigned int i = 0; i < actor->m_ModelComponent->m_Model->m_Meshes.size(); i++)
        {
            if (actor->m_ModelComponent->m_Model->m_Meshes[i]->m_VAO && actor->m_ModelComponent->m_Model->m_Meshes[i]
                ->m_IBO)
            {
                actor->m_ModelComponent->m_Model->m_Meshes[i]->m_VAO->Bind();
                actor->m_ModelComponent->m_Model->m_Meshes[i]->m_IBO->Bind();
                glDrawElements(GL_TRIANGLES, actor->m_ModelComponent->m_Model->m_Meshes[i]->m_IBO->m_IndexCount,
                               GL_UNSIGNED_INT, 0);
            }
        }*/
        DE_PROFILE_END(Render Scene)
    }

    void RendererSubsystem::RenderDefaultViewport() const
    {
        Ref<Scene> activeScene = m_ActiveScene.lock();
        if (!activeScene->m_ActiveCamera || !activeScene->m_ActiveCamera)
        {
            DE_LOG(LogRender, Error, "No Active Camera in Scene")
            return;
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        auto defaultShader = m_DefaultShader.lock();
        defaultShader->Bind();

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
        {
            if (!actor->m_RenderComponent->IsVisible() || !actor->m_RenderComponent->m_Material || !actor->
                m_ModelComponent->m_Model) continue;

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
                            actor->m_RenderComponent->m_Material->m_BaseColor.r,
                            actor->m_RenderComponent->m_Material->m_BaseColor.g,
                            actor->m_RenderComponent->m_Material->m_BaseColor.b);
            }


            // Upload the model matrix
            glUniformMatrix4fv(actor->m_RenderComponent->m_Shader->GetUniform("u_Model"), 1,
                               GL_FALSE, glm::value_ptr(actor->m_TransformComponent->m_Model));

            // Draw Call
            for (unsigned int i = 0; i < actor->m_ModelComponent->m_Model->m_Meshes.size(); i++)
            {
                if (actor->m_ModelComponent->m_Model->m_Meshes[i]->m_VAO && actor->m_ModelComponent->m_Model->m_Meshes[i]
                    ->m_IBO)
                {
                    actor->m_ModelComponent->m_Model->m_Meshes[i]->m_VAO->Bind();
                    actor->m_ModelComponent->m_Model->m_Meshes[i]->m_IBO->Bind();
                    glDrawElements(GL_TRIANGLES, actor->m_ModelComponent->m_Model->m_Meshes[i]->m_IBO->m_IndexCount,
                                   GL_UNSIGNED_INT, 0);
                }
            }
        }
    }

    void RendererSubsystem::SetActiveScene(const Ref<Scene>& _scene)
    {
        s_Instance->m_ActiveScene = _scene;
    }
}
