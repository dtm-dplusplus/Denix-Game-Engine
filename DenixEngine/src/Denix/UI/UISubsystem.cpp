#include "UISubsystem.h"

#include "backends/imgui_impl_sdl3.h"
#include "Denix/Core/File/FileSubsystem.h"

#include "Denix/Video/Window/Window.h"
#include "Denix/Video/WindowSubsystem.h"

namespace Denix
{
	void UISubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogUI, Warn, "UI Subsystem Initializing")
		FT_Error error;

		error = FT_Init_FreeType(&m_FtLibrary); /* initialize library */
		if (error)
		{
			DE_LOG(LogRender, Error, "Failed to initialize FreeType Library")
		}

		std::string fontFile = FileSubsystem::FormatPath("Content\\Engine\\fonts\\arial.ttf");

		error = FT_New_Face(UISubsystem::m_FtLibrary, fontFile.c_str(), 0, &m_Face); /* create face object */
		if (error == FT_Err_Unknown_File_Format)
		{
			DE_LOG(LogRender, Error, "Font format not supported")
		}
		else if (error)
		{
			DE_LOG(LogRender, Error, "Failed to load font")
		}
		
		DE_LOG(LogUI, Info, "UI Subsystem Initialized")
	}

	void UISubsystem::Deinitialize()
	{
		DE_LOG(LogUI, Trace, "UI Subsystem Deinitializing")

		FT_Done_Face(m_Face);
		FT_Done_FreeType(m_FtLibrary);
		Subsystem::Deinitialize();
		DE_LOG(LogUI, Trace, "UI Subsystem Deinitialized")
	}
	void UISubsystem::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
	{
		Subsystem::Update(_deltaTime, _waitCounter);

		//////////
		/*for (const Ref<Actor>& actor : activeScene->m_Actors)
        {
            if (!actor->m_RenderComponent->m_IsVisible || !actor->m_RenderComponent->m_Material || !actor->
                m_ModelComponent->m_Model) continue;

            // Base color/texture specific settings
            glUniform1i(actor->m_RenderComponent->m_Shader->GetUniform("u_Material.IsBaseTexture"),
                        actor->m_RenderComponent->m_Material->m_IsBaseTexture);

            if (actor->m_RenderComponent->m_Material->m_IsBaseTexture)
            {
                actor->m_RenderComponent->m_Material->m_BaseTexture->Bind();

                GLenum target = actor->m_RenderComponent->m_Material->m_BaseTexture->m_Target;
                glTexParameteri(target, GL_TEXTURE_WRAP_S, actor->m_RenderComponent->m_Material->m_TextureSettings.WrapMode);
                glTexParameteri(target, GL_TEXTURE_WRAP_T, actor->m_RenderComponent->m_Material->m_TextureSettings.WrapMode);
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, actor->m_RenderComponent->m_Material->m_TextureSettings.FilterMode);
                glTexParameteri(target, GL_TEXTURE_MAG_FILTER,actor->m_RenderComponent->m_Material->m_TextureSettings.FilterMode);
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
        }*/
	}
}
