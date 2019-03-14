#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

//include shape, shader header files
#include "GLerror.h"
#include "SDL_Start.h"
#include "Triangle_T.h"
#include "Circle.h"
#include "CircleTexture.h"
#include "Square.h"
#include "text.h"
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
#include "SDL_ttf.h"

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
SDL_Window *win;
bool windowOpen = true;
bool isFullScreen = false;
float bubbleSpeed = -0.001f;
float radius;
//screen boundaries for collision tests
float bX_r = 4.0f;
float bX_l = 0.0f;
float bY_t = 3.0f;
float bY_b = 0.0f;
//screen centre
float centreX = 2.0f;
float centreY = 1.5f;
//window aspect/width/height
int w;
int h;
float aspect;
int left;
int newwidth;
int newheight;
//text specific
const char* txtString;
int loopCounter = 0;

//transform matrices
glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;

glm::mat4 translate;
glm::mat4 circleTranslate;
glm::mat4 circleScale;
glm::mat4 rotate;
glm::mat4 scale;
glm::mat4 backgroundTranslate;
glm::mat4 backgroundScale;
glm::vec3 b_scaleFactor;
float angle = 0;

std::vector<CircleTexture> circles;

glm::vec3 lightCol;
float ambientIntensity;

//**************
//function prototypes
CircleTexture updatePositions(CircleTexture c);
void handleInput();

int main(int argc, char *argv[]) {
	//start and initialise SDL
	SDL_Start sdl;
	SDL_GLContext context = sdl.Init();
	win = sdl.win;

	SDL_GetWindowSize(win, &w, &h);
	glViewport(0, 0, w, h);
	aspect = (float)w / (float)h;

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//create triangle
	Triangle_T tri_T;
	//create background square
	Square background;

	//create circles
	float randValue, randValue2;
	srand(time(0));
	radius = 0.2f;

	for (int i = 0; i < 1; i++)
	{
		randValue = (float)rand() / RAND_MAX;
		randValue2 = (float)rand() / RAND_MAX;
		//circles.push_back(CircleTexture(radius, (randValue-0.5f), (randValue2 -0.5f)));
		circles.push_back(CircleTexture(radius, 0.0f, 0.0f));
	}

	errorLabel = 0;

	//*********************
	//create texture collection
	//create textures - space for 4, but only using 1
	Texture texArray[4];
	//background texture
	texArray[0].load("..//..//Assets//Textures//space.png");
	texArray[0].setBuffers();

	errorLabel = 2;

	//OpenGL buffers
	tri_T.setBuffers();
	background.setBuffers();
	//set buffers for the circles
	for (int q = 0; q < 1; q++)
	{
		circles[q].setBuffers();
	}

	errorLabel = 3;
	//*****************************************
	//set uniform variables
	int transformLocation;
	int modelLocation;
	int viewLocation;
	int projectionLocation;
	int circleTransformLocation;
	int backgroundColourLocation;
	int ambientIntensityLocation;

	//light colour initial setting
	lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
	//light distance setting
	ambientIntensity = 1.0f;

	//initialise transform matrices 
	//orthographic (2D) projection
	projectionMatrix = glm::ortho(0.0f, 4.0f, 0.0f, 3.0f, -1.0f, 100.0f);
	//initialise view matrix to '1'
	viewMatrix = glm::mat4(1.0f);

	circleTranslate = glm::mat4(1.0f);
	circleScale = glm::mat4(1.0f);
	translate = glm::mat4(1.0f);
	rotate = glm::mat4(1.0f);
	scale = glm::mat4(1.0f);
	backgroundScale = glm::mat4(1.0f);
	backgroundTranslate = glm::mat4(1.0f);

	//once only scale to background
	b_scaleFactor = { 20.0f, 15.0f, 1.0f };
	backgroundScale = glm::scale(backgroundScale, glm::vec3(b_scaleFactor));
	backgroundTranslate = glm::translate(backgroundTranslate, glm::vec3(2.0f, 1.5f, 0.0f));

	//once only scale to triangle
	scale = glm::scale(scale, glm::vec3(0.2f, 0.2f, 0.0f));

	
	circles[0].x = centreX;
	circles[0].y = centreY;
	circleScale = glm::scale(circleScale, glm::vec3(2.0f, 2.0f, 0.0f));

	//once only translate - translate triangle into centre of screen
	glUseProgram(tri_T.shaderProgram1);
	translate = glm::translate(translate, glm::vec3(2.0f, 1.5f, 0.0f));
	modelLocation = glGetUniformLocation(tri_T.shaderProgram1, "uModel");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(translate*rotate*scale));
	//once only translate - translate circle into centre of screen
	circleTranslate = glm::translate(circleTranslate, glm::vec3(centreX, centreY, 0.0f));

	errorLabel = 4;

	//on screen text
	std::string temp;
	//set what font is used - .ttf file in 'Fonts' folder
	Text osText("..//..//assets//fonts//cour.ttf");

	

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
		//background
		glUseProgram(background.shaderProgram);

		//set background lighting
		backgroundColourLocation = glGetUniformLocation(background.shaderProgram, "uLightColour");
		glProgramUniform3fv(background.shaderProgram, backgroundColourLocation, 1, glm::value_ptr(lightCol));
		//light distance
		ambientIntensityLocation = glGetUniformLocation(background.shaderProgram, "uAmbientIntensity");
		glProgramUniform1f(background.shaderProgram, ambientIntensityLocation, ambientIntensity);

		//set background image
		modelLocation = glGetUniformLocation(background.shaderProgram, "uModel");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(backgroundTranslate*backgroundScale));
		viewLocation = glGetUniformLocation(background.shaderProgram, "uView");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		projectionLocation = glGetUniformLocation(background.shaderProgram, "uProjection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glBindTexture(GL_TEXTURE_2D, texArray[0].texture);
		background.render();

		//specify shader program to use
		//to allow transform uniform to be passed in
		glUseProgram(tri_T.shaderProgram1);

		//set projection matrix uniform and other triangle values
		projectionLocation = glGetUniformLocation(tri_T.shaderProgram1, "uProjection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		modelLocation = glGetUniformLocation(tri_T.shaderProgram1, "uModel");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(translate*rotate*scale));
		viewLocation = glGetUniformLocation(tri_T.shaderProgram1, "uView");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		
		//draw the triangle with a shader and texture
		tri_T.render();

		//set to wireframe so we can see the circles
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//render the circles
		for (int q = 0; q < 1; q++)
		{
			circles[q] = updatePositions(circles[q]);

			glUseProgram(circles[q].shaderProgram);
			//set projection matrix uniform
			projectionLocation = glGetUniformLocation(circles[q].shaderProgram, "uProjection");
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
			modelLocation = glGetUniformLocation(circles[q].shaderProgram, "uModel");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(circleTranslate*circleScale));
			viewLocation = glGetUniformLocation(circles[q].shaderProgram, "uView");
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

			glBindVertexArray(circles[q].VAO);
			circles[q].render();
		}
		//render the text
		temp = std::to_string(loopCounter);
		//set text to display 'temp' and set it to a white colour
		//SDL uses 0-255 for colours rather that 0-1 in OpenGL
		//The text that is rendered could be anything - changed when an event happens for example.
		osText.setText(temp.c_str(), 255, 255, 255);
		osText.render();

		SDL_GL_SwapWindow(sdl.win);

		loopCounter++;
	}//end loop

	//****************************
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(context);

	SDL_Quit();
	return 0;
}

CircleTexture updatePositions(CircleTexture c)
{
	//update positions of the bubbles
	if (c.x < (bX_r-(radius*2))&(bubbleSpeed > 0))
	{
		circleTranslate = glm::translate(circleTranslate, glm::vec3(bubbleSpeed, 0.0f, 0.0f));
		c.x += bubbleSpeed;
		
		return c;
	}
	if (c.x > (bX_r - (radius * 2))&(bubbleSpeed > 0))
	{
		bubbleSpeed *= -1.0f;
		circleTranslate = glm::translate(circleTranslate, glm::vec3(bubbleSpeed, 0.0f, 0.0f));
		c.x += bubbleSpeed;
		
		return c;
	}
	if (c.x < (bX_r - (radius * 2))&(c.x > (bX_l + (radius * 2))&(bubbleSpeed < 0)))
	{
		
		circleTranslate = glm::translate(circleTranslate, glm::vec3(bubbleSpeed, 0.0f, 0.0f));
		c.x += bubbleSpeed;
		return c;
	}
	if (c.x < (bX_l + (radius * 2))&(bubbleSpeed < 0))
	{
		bubbleSpeed *= -1.0f;
		circleTranslate = glm::translate(circleTranslate, glm::vec3(bubbleSpeed, 0.0f, 0.0f));
		c.x += bubbleSpeed;
		
		return c;
	}
	if (c.x < (bX_r - (radius * 2))&(c.x > (bX_l - (radius * 2))&(bubbleSpeed > 0)))
	{

		circleTranslate = glm::translate(circleTranslate, glm::vec3(bubbleSpeed, 0.0f, 0.0f));
		c.x += bubbleSpeed;
		return c;
	}
	
	
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
		if (event.type == SDL_WINDOWEVENT)
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				std::cout << "Window resized w:" << w << " h:" << h << std::endl;
				SDL_GetWindowSize(win, &w, &h);
				newwidth = h * aspect;
				left = (w - newwidth) / 2;
				glViewport(left, 0, newwidth, h);
				break;
				

			default:
				break;
			}
		}
		
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			
			case SDLK_UP:
				translate = glm::translate(translate, glm::vec3((float)cos(angle)*0.02f, (float)sin(angle)*0.02f, 0.0f));
				break;
			case SDLK_a:
				angle += glm::radians(10.0f);
				rotate = glm::rotate(rotate,glm::radians(10.0f), glm::vec3(0, 0, 1));
				break;
			case SDLK_d:
				angle -= glm::radians(10.0f);
				rotate = glm::rotate(rotate, glm::radians(-10.0f) , glm::vec3(0, 0, 1));
				break;
			case SDLK_f:
				
				break;
			case SDLK_1:
				lightCol = glm::vec3(0.0f, 0.0f, 1.0f);
				break;
			case SDLK_2:
				lightCol = glm::vec3(0.0f, 1.0f, 0.0f);
				break;
			case SDLK_0:
				lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
				break;
			
			}
		}
	}
}
#endif
#endif
#endif
#endif