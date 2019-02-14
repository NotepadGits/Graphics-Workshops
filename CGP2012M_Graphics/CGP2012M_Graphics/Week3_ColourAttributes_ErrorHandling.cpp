#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

//include shape, shader header files
#include "GLerror.h"
#include "SDL_Start.h"
#include "Triangle.h"
#include "Circle.h"
#include "ShaderClass.h"

// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#ifndef GLEW_H
#define GLEW_H
//#include <GL/glew.h>
#include "windows.h"

// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#ifndef SDL_H
#define SDL_H
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
	//start and initialise SDL
	SDL_Start sdl;
	SDL_GLContext context = sdl.Init();

	//error class
	GLerror glerr;
	int errorLabel;

	//GLEW initialise
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	//register debug callback
	if (glDebugMessageCallback)
	{

		std::cout << "Registering OpenGL Debug callback function" << std::endl;
		glDebugMessageCallback(glerr.openglCallbackFunction, &errorLabel);
		glDebugMessageControl(GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DONT_CARE,
			0,
			NULL,
			true);
	}

	//*****************************************************
	//OpenGL specific data
	//create triangle
	Triangle tri;

	//create 10 circles
	float randValue, randValue2;
	srand(time(0));
	std::vector<Circle> circles;

	for (int i = 0; i < 5; i++)
	{
		randValue = (float)rand() / RAND_MAX;
		randValue2 = (float)rand() / RAND_MAX;
		circles.push_back(Circle(0.2f, (randValue-0.5f), (randValue2 -0.5f)));
	}

	errorLabel = 0;

	//create shaders
	Shader vSh("..//..//Assets//Shaders//shaderColour.vert");
	Shader fSh("..//..//Assets//Shaders//shaderColour.frag");
	Shader vSh1("..//..//Assets//Shaders//shader.vert");
	Shader fSh1("..//..//Assets//Shaders//shader.frag");

	//create, allocate and compile shaders
	//compile the shader code
	//1 for vertex, 2 for fragment - there is probably a better way to do this
	vSh.getShader(1);
	fSh.getShader(2);
	vSh1.getShader(1);
	fSh1.getShader(2);

	//create shader program, attach shaders together in the shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vSh.shaderID);
	glAttachShader(shaderProgram, fSh.shaderID);
	glLinkProgram(shaderProgram);

	GLuint shaderProgram1;
	shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vSh1.shaderID);
	glAttachShader(shaderProgram1, fSh1.shaderID);
	glLinkProgram(shaderProgram1);

	//delete shader source code pointers
	glDeleteShader(vSh.shaderID);
	glDeleteShader(fSh.shaderID);
	glDeleteShader(vSh1.shaderID);
	glDeleteShader(fSh1.shaderID);

	errorLabel = 2;

	//OpenGL buffers
	tri.setBuffers();
	//set buffers for the circles
	for (int q = 0; q < 5; q++)
	{
		circles[q].setBuffers();
	}

	errorLabel = 3;
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

		errorLabel = 4;

		/*while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL error: " << err << std::endl;
		}*/

		glUseProgram(shaderProgram1);
		glBindVertexArray(tri.VAO);
		tri.render();

		//draw the circles
		//Use shader program we have compiled and linked
		glUseProgram(shaderProgram);

		//set to wireframe so we can see the circles
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//render the circles
		for (int q = 0; q < 5; q++)
		{
			glBindVertexArray(circles[q].VAO);
			circles[q].render();
		}

		



		SDL_GL_SwapWindow(sdl.win);

		//*****************************
		//SDL handled input
		//Any input to the program is done here

		
			if (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					windowOpen = false;
				}
			}
		

	}
	//****************************
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(context);

	SDL_Quit();
	return 0;





}
#endif
#endif