#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

//include shape, shader header files
#include "GLerror.h"
#include "SDL_Start.h"
#include "Triangle_T.h"
#include "Circle.h"
#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H
#ifndef SHADERCLASS_H
#define SHADERCLASS_H

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
#include "SDL_image.h"
//#include "SDL_mixer.h"
//#include "SDL_ttf.h"

// // - OpenGL Mathematics - https://glm.g-truc.net/
#define GLM_FORCE_RADIANS // force glm to use radians
// // NOTE: must do before including GLM headers
// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//***************
//variables
SDL_Event event;
bool windowOpen = true;

glm::mat4 translate;
glm::mat4 circleTranslate;
glm::mat4 rotate;
glm::mat4 scale;
float angle = 0;

//**************
//function prototypes
void handleInput();

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
	Triangle_T tri_T;

	//circles have been commented out for clarity
	//create 10 circles
	float randValue, randValue2;
	srand(time(0));
	std::vector<Circle> circles;
	float radius = 0.2f;

	for (int i = 0; i < 1; i++)
	{
		randValue = (float)rand() / RAND_MAX;
		randValue2 = (float)rand() / RAND_MAX;
		circles.push_back(Circle(radius, (randValue-0.5f), (randValue2 -0.5f)));
	}

	errorLabel = 0;

	//create shaders
	Shader vSh("..//..//Assets//Shaders//shader_vColour_Transform.vert");
	Shader fSh("..//..//Assets//Shaders//shader_vColour_Transform.frag");
	

	//create, allocate and compile shaders
	//compile the shader code
	//1 for vertex, 2 for fragment - there is probably a better way to do this
	vSh.getShader(1);
	fSh.getShader(2);
	//

	////create shader program, attach shaders together in the shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vSh.shaderID);
	glAttachShader(shaderProgram, fSh.shaderID);
	glLinkProgram(shaderProgram);

	////delete shader source code pointers
	glDeleteShader(vSh.shaderID);
	glDeleteShader(fSh.shaderID);


	errorLabel = 2;

	//OpenGL buffers
	tri_T.setBuffers();
	//set buffers for the circles
	for (int q = 0; q < 1; q++)
	{
		circles[q].setBuffers();
	}

	errorLabel = 3;
	//*****************************************
	//set uniform variables
	int transformLocation;
	int circleTransformLocation;


	//initialise transform matrices 
	circleTranslate = glm::mat4(1.0f);
	translate = glm::mat4(1.0f);
	rotate = glm::mat4(1.0f);
	scale = glm::mat4(1.0f);

	//translate = glm::translate(translate, glm::vec3(0.2f, 0.3f, 0.0f));
	//rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0, 0, 1));
	scale = glm::scale(scale, glm::vec3(0.1f, 0.1f, 0.0f));

	errorLabel = 4;
	//*****************************
	//'game' loop
	while (windowOpen)
	{
		//*************************
		//process inputs
		handleInput();

		//****************************
		// OpenGL calls.
		
		glClearColor(1.0f, 1.0f, 1.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT); 

		errorLabel = 5;
		//specify shader program to use
		//to allow transform uniform to be passed in
		glUseProgram(tri_T.shaderProgram1);

		transformLocation = glGetUniformLocation(tri_T.shaderProgram1, "uTransform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(translate*rotate*scale));

		//draw the triangle with a shader and texture
		tri_T.render();

		//draw the circles
		//Use shader program we have compiled and linked
		glUseProgram(shaderProgram);
		circleTransformLocation = glGetUniformLocation(shaderProgram, "uTransform");
		circleTranslate = glm::translate(circleTranslate, glm::vec3(0.0f, 0.0f, 0.0f));
		
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(circleTranslate));
		//set to wireframe so we can see the circles
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//render the circles
		for (int q = 0; q < 1; q++)
		{
			glBindVertexArray(circles[q].VAO);
			circles[q].render();
		}

		SDL_GL_SwapWindow(sdl.win);

	}//end loop

	//****************************
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(context);

	SDL_Quit();
	return 0;
}

void handleInput()
{
	//*****************************
		//SDL handled input
		//Any input to the program is done here


	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			windowOpen = false;
		}
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				translate = glm::translate(translate, glm::vec3((float)cos(angle)*0.01f, (float)sin(angle)*0.01f, 0.0f));
				break;
			case SDLK_a:
				angle += glm::radians(10.0f);
				rotate = glm::rotate(rotate,glm::radians(10.0f), glm::vec3(0, 0, 1));
				break;
			case SDLK_d:
				angle -= glm::radians(10.0f);
				rotate = glm::rotate(rotate, glm::radians(-10.0f) , glm::vec3(0, 0, 1));
				break;
			
			}
		}
	}
}
#endif
#endif
#endif
#endif