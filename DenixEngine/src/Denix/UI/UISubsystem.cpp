#include "UISubsystem.h"

#include <SDL3/SDL_video.h>
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl3.h"
#include "Denix/Core/File/FileSubsystem.h"

#include "Denix/Video/Window/Window.h"
#include "Denix/Video/WindowSubsystem.h"



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
		
		FT_Face face;
        FT_GlyphSlot slot;
        FT_Matrix matrix = {0x10000, 0, 0, 0x10000};
        float m = 0x10000;
        FT_Vector pen; /* untransformed origin  */


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

        error = FT_New_Face(m_FtLibrary, filename, 0, &face); /* create face object */
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

        textureID = ConvertImageToTexture(image);
        
        FT_Done_Face(face);
		
		
		DE_LOG(LogUI, Info, "UI Subsystem Initialized")
	}

	void UISubsystem::Deinitialize()
	{
		DE_LOG(LogUI, Trace, "UI Subsystem Deinitializing")
		
		FT_Done_FreeType(m_FtLibrary);
		Subsystem::Deinitialize();
		DE_LOG(LogUI, Trace, "UI Subsystem Deinitialized")
	}
	void UISubsystem::Update(float _deltaTime)
	{
		ImGui::Begin("Renderer Subsystem");
		ImGui::Image((void*)(intptr_t)textureID, ImVec2(WIDTH, HEIGHT));
		ImGui::End();
	}
}
