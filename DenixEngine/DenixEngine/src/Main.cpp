//Using SDL and standard IO
#include <SDL.h>
#include <cstdio>
#include <exception>
#include <GL/glew.h>

int main(int argc, char* args[])
{
	//Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Window* gWindow = NULL;


	//Start up SDL and create window
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		//Create window
		gWindow = SDL_CreateWindow("Triangle",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		if (gWindow == NULL)
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());

		if (!SDL_GL_CreateContext(gWindow))
			throw std::exception();

		if (glewInit() != GLEW_OK)
			throw std::exception();
	}

	const GLfloat positions[] = {
		 0.0f, 0.5f, 0.0f,-0.5f,-0.5f, 0.0f,
		 0.5f,-0.5f, 0.0f
	};


	// VBO
	GLuint positionsVboId = 0;

	glGenBuffers(1, &positionsVboId);

	if (!positionsVboId)
	{
		throw std::exception();
	}
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
	const GLchar* vertexShaderSrc =
		"attribute vec3 in_Position; "\
		" "\
		"void main() "\
		"{ "\
		" gl_Position=vec4(in_Position,1.0);"\
		"} ";

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);

	glCompileShader(vertexShaderId);

	GLint success = 0;

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
		throw std::exception();

	// Create Fragment Shader
	const GLchar* fragmentShaderSrc =
		"void main() "\
		"{ "\
		" gl_FragColor=vec4(0,0,1,1);"\
		"} ";

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

	bool run = true;
	while (run)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) run = false;
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

		SDL_GL_SwapWindow(gWindow);
	}

	//Free resources and close SDL
	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}