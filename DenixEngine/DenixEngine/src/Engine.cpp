#include "DxPch.h"
#include "Engine.h"

#include <SDL.h>
#include <GL/glew.h>

#include "File.h"


Engine::Engine(): m_IsRunning{false}, m_Window{nullptr}
{
	m_WinX = 640;
	m_WinY = 480;
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
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			m_WinX, m_WinY,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		if (m_Window)
		{
			DX_LOG(DX_Log, DX_TRACE, "SDL_CreateWindow success")
		}
		else
		{
			DX_LOG(DX_Log, DX_CRITICAL, "SDL_CreateWindow failed! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		

		if (SDL_GL_CreateContext(m_Window))
		{
			DX_LOG(DX_Log, DX_TRACE, "SDL_GL_CreateContext success")
		}
		else
		{
			DX_LOG(DX_Log, DX_CRITICAL, "SDL_GL_CreateContext failed! SDL_Error: %s\n", SDL_GetError());
			return false;
		}

		if (glewInit() == GLEW_OK)
		{
			DX_LOG(DX_Log, DX_TRACE, "glewInit success")
		}
		else
		{
			DX_LOG(DX_Log, DX_CRITICAL, "glewInit failed! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
	}
	else
	{
		DX_LOG(DX_Log, DX_CRITICAL, "SDL_Init failed! SDL_Error: %s\n", SDL_GetError());
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

	// Create Vertex Shader
	const std::string vertexShaderStr = File::ReadFile("res/shaders/Vertex.glsl");
	const GLchar* vertexShaderSrc = vertexShaderStr.c_str();

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);

	glCompileShader(vertexShaderId);

	GLint success = 0;

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
		throw std::exception();

	// Create Fragment Shader
	const std::string fragmentShaderStr = File::ReadFile("res/shaders/Fragment.glsl");
	const GLchar* fragmentShaderSrc = fragmentShaderStr.c_str();

	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);

	glCompileShader(fragmentShaderId);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
		throw std::exception();

	//Create newshader program and attach shader objects
	GLuint programId = glCreateProgram();

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	glBindAttribLocation(programId, 0, "in_Position");

	//Performthelinkandcheck forfailure
	glLinkProgram(programId);

	glGetProgramiv(programId, GL_LINK_STATUS, &success);

	if (!success)
		throw std::exception();

	//Detach and destory shader objects
	glDetachShader(programId, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);

	while(m_IsRunning)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) m_IsRunning = false;
		}

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programId);
		glBindVertexArray(vaoId);
		//Draw3vertices(atriangle)
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//Resetthestate
		glBindVertexArray(0);
		glUseProgram(0);

		SDL_GL_SwapWindow(m_Window);
	}

	Stop();
}
