#include "DxPch.h"
#include "Engine.h"

#include <SDL.h>
#include <GL/glew.h>
// #include <SDL_opengl.h>

#include "File.h"
#include "Shader.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

Engine::Engine(): m_IsRunning{false}, m_Window{nullptr}, m_WinX{800}, m_WinY{600}
{
}



bool Engine::Start()
{
	Log::Start();
	DX_LOG(DX_Log, DX_TRACE, "Engine Starting")

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0)
	{
		DX_LOG(DX_Log, DX_CRITICAL, "SDL_Init failed! SDL_Error: %s\n", SDL_GetError())
		return false;
	}
	DX_LOG(DX_Log, DX_TRACE, "SDL_Init success")

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
		DX_LOG(DX_Log, DX_CRITICAL, "SDL_CreateWindow failed! SDL_Error: %s\n", SDL_GetError())
			return false;
	}
	DX_LOG(DX_Log, DX_TRACE, "SDL_CreateWindow success")

	// Create Context
	if (!SDL_GL_CreateContext(m_Window))
	{
		DX_LOG(DX_Log, DX_CRITICAL, "SDL_GL_CreateContext failed! SDL_Error: %s\n", SDL_GetError())
			return false;
	}
	DX_LOG(DX_Log, DX_TRACE, "SDL_GL_CreateContext success")

	SDL_GL_MakeCurrent(m_Window, SDL_GL_GetCurrentContext());
	SDL_GL_SetSwapInterval(1); // Enable vsync
	
	// Init Glew
	if (glewInit() != GLEW_OK)
	{
		DX_LOG(DX_Log, DX_CRITICAL, "glewInit failed! SDL_Error: %s\n", SDL_GetError())
			return false;
	}
	DX_LOG(DX_Log, DX_TRACE, "glewInit success")

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
	// DX_LOG(Test, DX_TRACE, "Engine Stopping") - Memory Error on force close

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());
	SDL_DestroyWindow(m_Window);
	SDL_Quit();

	//Log::Stop();
}

void Engine::Run()
{
	if(!Start()) throw std::exception();

	DX_LOG_CREATE(GL_Log)
	const GLfloat positions[] = {
		 0.0f, 0.5f, 0.0f,-0.5f,-0.5f, 0.0f,
		 0.5f,-0.5f, 0.0f
	};
	DX_LOG(GL_Log, DX_TRACE, "Created triangle Postitions")

	// VBO
	GLuint positionsVboId = 0;

	glGenBuffers(1, &positionsVboId);

	if (!positionsVboId)
		DX_LOG(GL_Log, DX_ERROR, "Failed to create VBO")
	DX_LOG(GL_Log, DX_TRACE, "Created VBO")

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);
	DX_LOG(GL_Log, DX_TRACE, "Bound VBO")

	// Upload a copy of the data from memory into the new VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	DX_LOG(GL_Log, DX_TRACE, "Uploaded data to VBO")

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	DX_LOG(GL_Log, DX_TRACE, "Reset VBO state")

	GLuint vaoId = 0;

	// Create a new VAO on the GPU and bind it
	glGenVertexArrays(1, &vaoId);
	if (!vaoId)
	{
		DX_LOG(GL_Log, DX_ERROR, "Failed to create VAO")
	}
	glBindVertexArray(vaoId);
	DX_LOG(GL_Log, DX_TRACE, "Created and bound VAO")

	// Bind the position VBO, assign it to position 0 on the bound VAO
	// and flag it to be used
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);
	DX_LOG(GL_Log, DX_TRACE, "Bound VBO")

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GLfloat), (void*)0);
	DX_LOG(GL_Log, DX_TRACE, "Assigned VBO to VAO")

	glEnableVertexAttribArray(0);
	DX_LOG(GL_Log, DX_TRACE, "Enabled VAO")

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	DX_LOG(GL_Log, DX_TRACE, "Reset VAO state")

	// Create ShaderProgram
	ShaderProgram shaderProgram;
	shaderProgram.CreateProgram();
	shaderProgram.AttachShader(Shader(GL_VERTEX_SHADER, File::Read("res/shaders/Vert.glsl")));
	shaderProgram.AttachShader(Shader(GL_FRAGMENT_SHADER, File::Read("res/shaders/Frag.glsl")));
	shaderProgram.LinkProgram();
	glBindAttribLocation(shaderProgram.GetProgram(), 0, "in_Position");


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
			glUseProgram(shaderProgram.GetProgram());
			glBindVertexArray(vaoId);
			//Draw3vertices(atriangle)
			glDrawArrays(GL_TRIANGLES, 0, 3);
			//Resetthestate
			glBindVertexArray(0);
			glUseProgram(0);
		}

		// Demo window
		static bool showDemo = true;
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
