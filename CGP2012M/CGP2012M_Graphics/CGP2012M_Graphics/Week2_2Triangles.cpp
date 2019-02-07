#include <cstdio>
#include <cstdlib>
#include <iostream>

// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#include <GL/glew.h>
#include "windows.h"

// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#include "SDL.h"
//#include "SDL_image.h"
//#include "SDL_mixer.h"
// "SDL_ttf.h"

// // - OpenGL Mathematics - https://glm.g-truc.net/
#define GLM_FORCE_RADIANS // force glm to use radians
// // NOTE: must do before including GLM headers
// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



int main(int argc, char *argv[]) {
	//SDL Initialise
	SDL_Init(SDL_INIT_EVERYTHING);

	//SDL create window
	SDL_Window *win = SDL_CreateWindow("OpenGL Window", 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	//set context attributes
	//sets opengl version to 4.3
	int major = 4, minor = 3;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_CORE); //use core profile
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); //ask for forward compatible
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(win);

	//GLEW initialise
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	//*****************************************************
	//OpenGL specific data
	//single array of all vertices
	GLfloat vertices[] = {
		-0.9f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		0.45f,  0.5f, 0.0f,
		// Second triangle
		0.0f, -0.5f, 0.0f,
		0.9f, -0.5f, 0.0f,
		0.7f, 0.5f, 0.0f
	};

	//vertex shader source
	GLchar const *vShader = { "#version 440 core\nlayout(location=0) in vec3 position;\nvoid main()\n{ gl_Position = vec4(position.x, position.y, position.z, 1.0);}\n" };
	//fragment shader source
	GLchar const *fShader = { "#version 440 core\nout vec4 color;\nvoid main()\n{ color = vec4(0.0f, 0.0f, 1.0f, 1.0f);}\n" };

	//create, allocate and compile shaders
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShader, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShader, NULL);
	glCompileShader(fragmentShader);

	//create shader program, attach shaders together in the shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//delete shader source code pointers
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//OpenGL buffers
	//set up for the shapes 
	GLuint VBO;
	glGenBuffers(1, &VBO);
	// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	//bind the Vertex Array Object (VAO)
	glBindVertexArray(VAO);
	//copy the vertices array into a buffer (VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Unbind the VAO
	glBindVertexArray(0);
	//***********************************************

	SDL_Event event;
	bool windowOpen = true;

	//*****************************
	//'game' loop
	while (windowOpen)
	{

		//****************************
		// OpenGL calls.
		
		glClearColor(1.0f, 0.0f, 0.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT); 

		//draw the triangles
		//Use shader program we have compiled and linked
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//set to wireframe so we can see the 2 triangles
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//drawing 6 vertices (2 triangles)
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//unbind (release) the VAO
		glBindVertexArray(0);

		SDL_GL_SwapWindow(win);


		//*****************************
		//SDL handled input
		//Any input to the program is done here

		while (windowOpen)
		{
			if (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					windowOpen = false;
				}
			}
		}

	}
	//****************************
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(glcontext);

	SDL_Quit();
	return 0;





}