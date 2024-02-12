#include "DePch.h"
#include "Engine.h"

#include <SDL.h>
#include <GL/glew.h>
// #include <SDL_opengl.h>

#include "File.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

Engine::Engine(): m_IsRunning{false}, m_Window{nullptr}, m_WinX{800}, m_WinY{600}
{
}


bool Engine::Start()
{
	DeLog::Start();
	DE_LOG(Log, Trace, "Engine Starting")

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0)
	{
		DE_LOG(Log, Critical, "SDL_Init failed! SDL_Error: %s\n", SDL_GetError())
		return false;
	}
	DE_LOG(Log, Trace, "SDL_Init success")

	// Set SDL OpenGL Version
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Useful for virtual keyboards, complex chararacter sets
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	// Set GL Attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//Create window
	SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	m_Window = SDL_CreateWindow("Denix Engine",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_WinX, m_WinY, windowFlags);

	if (!m_Window)
	{
		DE_LOG(Log, Critical, "SDL_CreateWindow failed! SDL_Error: %s\n", SDL_GetError())
			return false;
	}
	DE_LOG(Log, Trace, "SDL_CreateWindow success")

	// Create Context
	if (!SDL_GL_CreateContext(m_Window))
	{
		DE_LOG(Log, Critical, "SDL_GL_CreateContext failed! SDL_Error: %s\n", SDL_GetError())
			return false;
	}
	DE_LOG(Log, Trace, "SDL_GL_CreateContext success")

	SDL_GL_MakeCurrent(m_Window, SDL_GL_GetCurrentContext());
	SDL_GL_SetSwapInterval(1); // Enable vsync
	
	// Init Glew
	if (glewInit() != GLEW_OK)
	{
		DE_LOG(Log, Critical, "glewInit failed! SDL_Error: %s\n", SDL_GetError())
			return false;
	}
	DE_LOG(Log, Trace, "glewInit success")

	// Enable Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(m_Window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init(glsl_version);

	m_IsRunning = true;

	return true;
}

void Engine::Stop()
{
	// DE_LOG(Test, Trace, "Engine Stopping") - Memory Error on force close

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());
	SDL_DestroyWindow(m_Window);
	SDL_Quit();

	//DeLog::Stop();
}

void Engine::Run()
{
	if(!Start()) throw std::exception();

	const GLfloat positions[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f,-0.5f, 0.0f,
		0.5f,-0.5f, 0.0f
	};

	const GLfloat colors[] = {
	  1.0f, 0.0f, 0.0f, 1.0f,
	  0.0f, 1.0f, 0.0f, 1.0f,
	  0.0f, 0.0f, 1.0f, 1.0f,
	};

	// Manual VBO creation
	// Postion VBO
	VertexBuffer positionsVB;
	positionsVB.GenGlID();
	positionsVB.Bind();
	positionsVB.BufferData(positions, sizeof(positions));
	positionsVB.Unbind();
	
	// Automatic VBO creation
	// Colors VBO
	VertexBuffer colorVB(colors, sizeof(colors));

	// Create a new VAO on the GPU and bind it
	GLuint vaoId = 0;
	{
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
		if (!vaoId) DE_LOG(LogGL, Error, "Failed to create VAO")

		// Bind the position VBO, assign it to position 0 on the bound VAO
		{
			positionsVB.Bind();
			DE_LOG(LogGL, Trace, "Bound positions VBO")

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
				3 * sizeof(GLfloat), (void*)0);
			DE_LOG(LogGL, Trace, "Assigned positions VBO to VAO")

			glEnableVertexAttribArray(0);
			DE_LOG(LogGL, Trace, "Enabled Position VBO Index in VAO")
		}

		// Bind the color VBO, assign it to position 1 on the bound VAO
		{
			colorVB.Bind();
			DE_LOG(LogGL, Trace, "Bound colors VBO")

			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
				4 * sizeof(GLfloat), (void*)0);

			glEnableVertexAttribArray(1);
			DE_LOG(LogGL, Trace, "Enabled Color VBO Index in VAO")
		}
		

		// Reset the state
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		DE_LOG(LogGL, Trace, "Reset VAO state")
	}

	// Create ShaderProgram
	ShaderProgram shaderProgram;
	{
		shaderProgram.CreateProgram();
		shaderProgram.AttachShader(Shader(GL_VERTEX_SHADER, File::Read("res/shaders/Vert.glsl")));
		shaderProgram.AttachShader(Shader(GL_FRAGMENT_SHADER, File::Read("res/shaders/Frag.glsl")));

		glBindAttribLocation(shaderProgram.GetProgramID(), 0, "a_Position");
		glBindAttribLocation(shaderProgram.GetProgramID(), 1, "a_Color");

		shaderProgram.LinkProgram();
	}
	
	/*GLint colorUniformId = glGetUniformLocation(shaderProgram.GetProgramID(), "u_Color");
	glm::vec4 color = { 0.0f, 1.0f, 0.0f, 1.0f };
	if (colorUniformId == -1)
		DE_LOG(GL_Log, Error, "Enabled VAO")*/

	// Main loop
	while(m_IsRunning)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			ImGui_ImplSDL2_ProcessEvent(&e);
			if (e.type == SDL_QUIT)
				m_IsRunning = false;
			if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE
				&& e.window.windowID == SDL_GetWindowID(m_Window))
				m_IsRunning = false;
		}

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// New ImGui frame
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
		}

		// Triangle
		{
			glUseProgram(shaderProgram.GetProgramID());
			glBindVertexArray(vaoId);
			//Draw3vertices(atriangle)
			// ImGui::Begin("Triangle Color");
			// ImGui::ColorEdit4("RGBA", &color[0]);
			// ImGui::End();
			// glUniform4f(colorUniformId, color[0], color[1], color[2], color[3]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			//Resetthestate
			glBindVertexArray(0);
			glUseProgram(0);
		}

		// Demo window
		static bool showDemo = false;
		if (showDemo)
			ImGui::ShowDemoWindow(&showDemo);

		//glViewport(0, 0, m_WinX, m_WinY); Neccessary?

		// ImGui Render
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		
		SDL_GL_SwapWindow(m_Window);
	}

	Stop();
}
