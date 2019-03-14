#include <cstdio>
#include <cstdlib>
#include <iostream>

//include shape, shader header files
#include "Triangle.h"
#include "ShaderClass.h"

// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#ifndef GLEW_H
#define GLEW_H
//#include <GL/glew.h>
#include "windows.h"

// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#include "SDL.h"
//#include "SDL_image.h"
//#include "SDL_mixer.h"
//#include "SDL_ttf.h"

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
	//create objects
	Triangle tri;

	//create shaders
	Shader vSh("..//..//Assets//Shaders//shader.vert");
	Shader fSh("..//..//Assets//Shaders//shader.frag");

	//create, allocate and compile shaders
	//compile the shader code
	//1 for vertex, 2 for fragment - there is probably a better way to do this
	vSh.getShader(1);
	fSh.getShader(2);

	//create shader program, attach shaders together in the shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vSh.shaderID);
	glAttachShader(shaderProgram, fSh.shaderID);
	glLinkProgram(shaderProgram);

	//delete shader source code pointers
	glDeleteShader(vSh.shaderID);
	glDeleteShader(fSh.shaderID);

	//OpenGL buffers
	//set buffers for the triangle
	tri.setBuffers();
	
	//***********************************************

	SDL_Event event;
	bool windowOpen = true;

	//*****************************
	//'game' loop
	while (windowOpen)
	{

		//****************************
		// OpenGL calls.
		
		glClearColor(1.0f, 1.0f, 1.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT); 

		//draw the triangles
		//Use shader program we have compiled and linked
		glUseProgram(shaderProgram);

		//set to wireframe so we can see the 2 triangles
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		tri.render();

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
#endif