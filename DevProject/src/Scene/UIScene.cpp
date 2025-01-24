#include "UIScene.h"

#include "imgui.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/UI/UISubsystem.h"

unsigned char image[HEIGHT][WIDTH];

std::vector<std::string> text = { "Tex 1", "Tex 2" }; 

// Function to convert the image to an OpenGL texture
GLuint ConvertImageToTexture(unsigned char image[HEIGHT][WIDTH]) {
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the image data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIDTH, HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, image);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
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
            if (i < 0 || j < 0 ||i >= WIDTH || j >= HEIGHT) continue;
                
            image[j][i] |= bitmap->buffer[q * bitmap->width + p];
        }
    }
}




void Denix::UIScene::BeginScene()
{
    Scene::BeginScene();
    canvas = MakeRef<Canvas>();
    m_ActiveCamera->GetTransformComponent()->GetTransform().Position = {0.0f, 0.0f, 0.0f};
   // CreateTextBox("", Position, {WIDTH, HEIGHT}, {1.0f, 1.0f, 1.0f});
}

void Denix::UIScene::Update(float _deltaTime)
{
    Scene::Update(_deltaTime);

    ImGui::Begin("Font Rendering");
    /*for (int i = 0; i < 2; i++)
    {
        if (textureID[i]) ImGui::Image((void*)(intptr_t)textureID[i], ImVec2(WIDTH, HEIGHT));
    }*/

    if (ImGui::Button("Create Button"))
    {
        Ref<Button> btn = SpawnActor<Button>();
        btn->GetTransformComponent()->SetRotation(90.0f, 0.0f, 0.0f);
        canvas->m_Buttons.push_back(btn);
    }
    if (ImGui::Button("Sort Buttons"))
    {
        canvas->Sort();
    }
    if (ImGui::Button("Print Button"))
    {
       for (int i = 0; i < canvas->m_Buttons.size(); i++) 
       {
           DE_LOG(LogRender, Info, "{}: {}", canvas->m_Buttons[i]->GetName(),
               canvas->m_Buttons[i]->GetTransformComponent()->GetTransform().Position.y);
       }
    }
    ImGui::End();

    canvas->Update();
}

unsigned int Denix::UIScene::CreateTextBox(std::string _text, glm::vec2 _position, glm::vec2 _size, glm::vec3 _color)
{
    FT_Face& face = UISubsystem::m_Face;
        /* first argument     */
        double angle = 0.0f; //(25.0 / 360) * 3.14159 * 2; /* use 25 degrees     */
        int target_height = HEIGHT;
       
        /* use 50pt at 100dpi */
        FT_Error error = FT_Set_Char_Size(face, 50 * 64, 0,
                                 100, 0); /* set character size */
        if (error)
        {
            DE_LOG(LogRender, Error, "Failed to set character size")
        }

        FT_GlyphSlot slot = face->glyph;
        /* cmap selection omitted;                                        */
        /* for simplicity we assume that the font contains a Unicode cmap */

        for (int i = 0; i < text.size(); i++)
        {
            FT_Matrix matrix; /* transformation matrix */
            FT_Vector pen; /* untransformed origin  */
            
            /* set up matrix */
            matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
            matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
            matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
            matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);

            /* the pen position in 26.6 cartesian space coordinates; */
            /* start at (300,200) relative to the upper left corner  */
            pen.x = _position.x * 64;
            pen.y = (target_height- _position.y) * 64;
            for (const char n : text[i])
            {
                /* set transformation */
                FT_Set_Transform(face, &matrix, &pen);

                /* load glyph image into the slot (erase previous one) */
                error = FT_Load_Char(face, n, FT_LOAD_RENDER);
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

            textureID[i] = ConvertImageToTexture(image);
            memset(image, 0, sizeof(image));
        }
    return 0;
}
