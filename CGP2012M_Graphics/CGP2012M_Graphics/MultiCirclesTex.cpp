#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

//include shape, shader header files
#include "SDL_Start.h"
#include "Triangle.h"
#include "CircleTexture.h"
#include "ShaderClass.h"
#include "TextureClass.h"

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

	//GLEW initialise
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	//*****************************************************
	//OpenGL specific data
	//create 10 circles
	float randValue, randValue2;
	srand(time(0));
	std::vector<CircleTexture> circles;

	for (int i = 0; i < 5; i++)
	{
		randValue = (float)rand() / RAND_MAX;
		randValue2 = (float)rand() / RAND_MAX;
		circles.push_back(CircleTexture(0.2f, (randValue-0.5f), (randValue2 -0.5f)));
	}

	//create texture
	Texture tex;
	tex.load("..//..//Assets//Textures//circlePattern_alpha.png");
	//create shaders
	Shader vSh("..//..//Assets//Shaders//shader_vColour_Texture.vert");
	Shader fSh("..//..//Assets//Shaders//shader_vColour_Texture.frag");

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
	tex.setBuffers();
	//set buffers for the circles
	for (int q = 0; q < 5; q++)
	{
		circles[q].setBuffers();
	}
	
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

		//draw the circles
		//Use shader program we have compiled and linked
		glUseProgram(shaderProgram);

		//set to wireframe so we can see the circles
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindTexture(GL_TEXTURE_2D, tex.texture);
		//render the circles
		for (int q = 0; q < 5; q++)
		{
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