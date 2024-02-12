#include "DePch.h"
#include "Engine.h"

#include <SDL.h>
#include <GL/glew.h>

#include "File.h"
#include "Video/GL/VertexBuffer.h"
#include "Video/GL/Shader.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "Video/GL/VertexArray.h"

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
		DE_LOG(Log, Critical, "SDL_Init failed! SDL_Error: {}", SDL_GetError())
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
		DE_LOG(Log, Critical, "SDL_CreateWindow failed! SDL_Error: {}", SDL_GetError())
			return false;
	}
	DE_LOG(Log, Trace, "SDL_CreateWindow success")

	// Create Context
	if (!SDL_GL_CreateContext(m_Window))
	{
		DE_LOG(Log, Critical, "SDL_GL_CreateContext failed! SDL_Error: {}", SDL_GetError())
			return false;
	}
	DE_LOG(Log, Trace, "SDL_GL_CreateContext success")

	SDL_GL_MakeCurrent(m_Window, SDL_GL_GetCurrentContext());
	SDL_GL_SetSwapInterval(1); // Enable vsync
	
	// Init Glew
	if (glewInit() != GLEW_OK)
	{
		DE_LOG(Log, Critical, "glewInit failed! SDL_Error: {}", SDL_GetError())
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

	DE_LOG(Log, Info, "Engine Started")
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

	// VBO creation
	const VertexBuffer positionsVB(positions, sizeof(positions));
	const VertexBuffer colorVB(colors, sizeof(colors));

	// Create a new VAO on the GPU and bind it
	const std::unique_ptr<VertexArray> vao = std::make_unique<VertexArray>();
	vao->GenVertexArray();
	vao->Bind();

	// Bind the vbos & attrivbs
	positionsVB.Bind();
	vao->AttribPtr(0, 3, GL_FLOAT);

	colorVB.Bind();
	vao->AttribPtr(1, 4, GL_FLOAT);

	// Reset the state
	VertexBuffer::Unbind();
	VertexArray::Unbind();

	// Create ShaderProgram
	const std::unique_ptr<ShaderProgram> shaderProgram = std::make_unique<ShaderProgram>();
	shaderProgram->CreateProgram();
	shaderProgram->CompileShader(GL_VERTEX_SHADER, File::Read("res/shaders/Vert.glsl"));
	shaderProgram->CompileShader(GL_FRAGMENT_SHADER, File::Read("res/shaders/Frag.glsl"));
	shaderProgram->AttachShaders();

	shaderProgram->BindAttrib(0, "a_Position");
	shaderProgram->BindAttrib(1, "a_Color");

	if(!shaderProgram->LinkProgram()) DE_LOG(LogGL, Error, "Link program failed")

	// Main loop
	while(m_IsRunning)
	{
		// Input Poll - Move to input class
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

		// Clear buffer, move to renderer
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// New ImGui frame - Move to UI class
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// Triangle - Move to scene class
		shaderProgram->Bind();
		vao->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		VertexArray::Unbind();
		ShaderProgram::Unbind();

		// Render - Move to Render Class/UI Class
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(m_Window);
	}

	Stop();
}
