#include "RendererSubsystem.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Scene/Camera.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Scene/Actor.h"
#include "Denix/Physics/Collider.h"
#include "Denix/Profile/ProfileSubsystem.h"

#include <ft2build.h>

#include "imgui.h"
#include "Denix/Core/File/FileSubsystem.h"

#include FT_FREETYPE_H

#define WIDTH   640
#define HEIGHT  480

/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];
GLuint textureID;
#include <GL/glew.h>

// Function to convert the image to an OpenGL texture
GLuint ConvertImageToTexture(unsigned char image[HEIGHT][WIDTH]) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the image data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIDTH, HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, image);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}
/* Replace this function with something useful. */

void
draw_bitmap(FT_Bitmap* bitmap,
            FT_Int x,
            FT_Int y)
{
    FT_Int i, j, p, q;
    FT_Int x_max = x + bitmap->width;
    FT_Int y_max = y + bitmap->rows;


    /* for simplicity, we assume that `bitmap->pixel_mode' */
    /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */

    for (i = x, p = 0; i < x_max; i++, p++)
    {
        for (j = y, q = 0; j < y_max; j++, q++)
        {
            if (i < 0 || j < 0 ||
                i >= WIDTH || j >= HEIGHT)
                continue;

            image[j][i] |= bitmap->buffer[q * bitmap->width + p];
        }
    }
}


void
show_image(void)
{
    int i, j;


    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
            putchar(image[i][j] == 0
                        ? ' '
                        : image[i][j] < 128
                        ? '+'
                        : '*');
        putchar('\n');
    }
}


namespace Denix
{
    void RendererSubsystem::Initialize()
    {
        Subsystem::Initialize();
        DE_LOG(LogRender, Warn, "Initializing RendererSubsystem")
        m_DefaultShader = AssetSubsystem::GetShader("DefaultShader");
        DE_LOG(LogRender, Info, "RendererSubsystem Initialized")

        FT_Library library;
        FT_Face face;

        FT_GlyphSlot slot;
        FT_Matrix matrix = {0x10000, 0, 0, 0x10000};
        float m = 0x10000;
        FT_Vector pen; /* untransformed origin  */
        FT_Error error;


        double angle;
        int target_height;
        int n, num_chars;


        /*if ( argc != 3 )
        {
          fprintf ( stderr, "usage: %s font sample-text\n", argv[0] );
          exit( 1 );
        }*/

        std::string font_name = FileSubsystem::FormatPath("Content/Engine/fonts/arial.ttf");
        const char* filename = font_name.c_str();
        /* first argument     */
        const char* text = "Hello World"; /* second argument    */
        num_chars = strlen(text);
        angle = 0.0f; //(25.0 / 360) * 3.14159 * 2; /* use 25 degrees     */
        target_height = HEIGHT;

        error = FT_Init_FreeType(&library); /* initialize library */
        if (error)
        {
            DE_LOG(LogRender, Error, "Failed to initialize FreeType Library")
        }

        error = FT_New_Face(library, filename, 0, &face); /* create face object */
        if (error == FT_Err_Unknown_File_Format)
        {
            DE_LOG(LogRender, Error, "Font format not supported")
        }
        else if (error)
        {
            DE_LOG(LogRender, Error, "Failed to load font")
        }
        
        /* use 50pt at 100dpi */
        error = FT_Set_Char_Size(face, 50 * 64, 0,
                                 100, 0); /* set character size */
        if (error)
        {
            DE_LOG(LogRender, Error, "Failed to set character size")
        }
        /* cmap selection omitted;                                        */
        /* for simplicity we assume that the font contains a Unicode cmap */

        slot = face->glyph;

        /* set up matrix */
        matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
        matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
        matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
        matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);

        /* the pen position in 26.6 cartesian space coordinates; */
        /* start at (300,200) relative to the upper left corner  */
        //pen.x = 300 * 64;
        // pen.y = (target_height - 200) * 64;
        pen.x = 220 * 64;
        pen.y = (target_height- 240) * 64;
        for (n = 0; n < num_chars; n++)
        {
            /* set transformation */
            FT_Set_Transform(face, &matrix, &pen);

            /* load glyph image into the slot (erase previous one) */
            error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
            if (error)
                continue; /* ignore errors */

            /* now, draw to our target surface (convert position) */
            draw_bitmap(&slot->bitmap,
                        slot->bitmap_left,
                        target_height - slot->bitmap_top);

            /* increment pen position */
            pen.x += slot->advance.x;
            pen.y += slot->advance.y;
        }

        //show_image();

        textureID = ConvertImageToTexture(image);
        
        FT_Done_Face(face);
        FT_Done_FreeType(library);
    }

    void RendererSubsystem::Deinitialize()
    {
        DE_LOG(LogRender, Trace, "RendererSubsystem Deinitializing")
        DE_LOG(LogRender, Trace, "RendererSubsystem Deinitialized")
    }

    void RendererSubsystem::RenderScene()
    {
        DE_PROFILE(Render Scene)

        if (!m_Enabled)
        {
            DE_PROFILE_END(Render Scene)
            return;
        }

        RenderDefaultViewport();

        ImGui::Begin("Renderer Subsystem");
        ImGui::Image((void*)(intptr_t)textureID, ImVec2(WIDTH, HEIGHT));
        ImGui::End();
        DE_PROFILE_END(Render Scene)
    }

    void RendererSubsystem::RenderDefaultViewport() const
    {
        Ref<Scene> activeScene = m_ActiveScene.lock();
        if (!activeScene->m_ActiveCamera || !activeScene->m_ActiveCamera->m_CameraComponent)
        {
            DE_LOG(LogRender, Error, "No Active Camera in Scene")
            return;
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        auto defaultShader = m_DefaultShader.lock();
        defaultShader->Bind();

        // Upload the camera matrices relative to Object
        glUniformMatrix4fv(defaultShader->GetUniform("u_Projection"), 1,
                           GL_FALSE, glm::value_ptr(
                               activeScene->m_ActiveCamera->m_CameraComponent->m_Projection));

        glUniformMatrix4fv(defaultShader->GetUniform("u_View"), 1,
                           GL_FALSE, glm::value_ptr(
                               activeScene->m_ActiveCamera->m_CameraComponent->m_View));

        for (const Ref<Actor>& actor : activeScene->m_Actors)
        {
            if (!actor->m_RenderComponent->IsVisible() || !actor->m_RenderComponent->m_Material || !actor->
                m_MeshComponent->m_Model) continue;

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
            for (unsigned int i = 0; i < actor->m_MeshComponent->m_Model->m_Meshes.size(); i++)
            {
                if (actor->m_MeshComponent->m_Model->m_Meshes[i]->m_VAO && actor->m_MeshComponent->m_Model->m_Meshes[i]
                    ->m_IBO)
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
        s_Instance->m_ActiveScene = _scene;
    }
}
