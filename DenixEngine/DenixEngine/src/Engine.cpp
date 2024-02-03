#include "DxPch.h"
#include "Engine.h"

#include <SDL.h>
#include <GL/glew.h>

#include "File.h"
#include "Shader.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

Engine::Engine(): m_IsRunning{false}, m_Window{nullptr}
{
	m_WinX = 800;
	m_WinY = 600;
}

Engine::~Engine() = default;


bool Engine::Start()
{
	Log::Start();
	DX_LOG(DX_Log, DX_TRACE, "Engine Starting")

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) >= 0)
	{
		DX_LOG(DX_Log, DX_TRACE, "SDL_Init success")

		//Create window
		m_Window = SDL_CreateWindow("Denix Engine",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			m_WinX, m_WinY,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		if (m_Window)
		{
			DX_LOG(DX_Log, DX_TRACE, "SDL_CreateWindow success")
		}
		else
		{
			DX_LOG(DX_Log, DX_CRITICAL, "SDL_CreateWindow failed! SDL_Error: %s\n", SDL_GetError())
			return false;
		}
		

		if (SDL_GL_CreateContext(m_Window))
		{
			DX_LOG(DX_Log, DX_TRACE, "SDL_GL_CreateContext success")
		}
		else
		{
			DX_LOG(DX_Log, DX_CRITICAL, "SDL_GL_CreateContext failed! SDL_Error: %s\n", SDL_GetError())
			return false;
		}

		if (glewInit() == GLEW_OK)
		{
			DX_LOG(DX_Log, DX_TRACE, "glewInit success")
		}
		else
		{
			DX_LOG(DX_Log, DX_CRITICAL, "glewInit failed! SDL_Error: %s\n", SDL_GetError())
			return false;
		}
	}
	else
	{
		DX_LOG(DX_Log, DX_CRITICAL, "SDL_Init failed! SDL_Error: %s\n", SDL_GetError())
		return false;
	}

	m_IsRunning = true;

	return true;
}

void Engine::Stop()
{
	DX_LOG(Test, DX_TRACE, "Engine Stopping")

	SDL_DestroyWindow(m_Window);
	SDL_Quit();


	Log::Stop();
}

void Engine::Run()
{
	if(!Start()) throw std::exception();

	const GLfloat positions[] = {
		 0.0f, 0.5f, 0.0f,-0.5f,-0.5f, 0.0f,
		 0.5f,-0.5f, 0.0f
	};


	// VBO
	GLuint positionsVboId = 0;

	glGenBuffers(1, &positionsVboId);

	if (!positionsVboId)
		throw std::exception();

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);

	// Upload a copy of the data from memory into the new VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint vaoId = 0;

	// Create a new VAO on the GPU and bind it
	glGenVertexArrays(1, &vaoId);
	if (!vaoId)
	{
		throw std::exception();
	}
	glBindVertexArray(vaoId);

	// Bind the position VBO, assign it to position 0 on the bound VAO
	// and flag it to be used
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(0);

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Create ShaderProgram
	ShaderProgram shaderProgram;
	shaderProgram.CreateProgram();
	shaderProgram.AttachShader(Shader(GL_VERTEX_SHADER, File::Read("res/shaders/Vert.glsl")));
	shaderProgram.AttachShader(Shader(GL_FRAGMENT_SHADER, File::Read("res/shaders/Frag.glsl")));
	shaderProgram.LinkProgram();
	glBindAttribLocation(shaderProgram.GetProgram(), 0, "in_Position");


	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(m_Window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init("#version 460");

	bool showDemo = true;

	// Main loop
	while(m_IsRunning)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) m_IsRunning = false;
		}

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplSDL2_NewFrame(m_Window);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

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

		ImGui::ShowDemoWindow(&showDemo);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_SwapWindow(m_Window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	Stop();
}
