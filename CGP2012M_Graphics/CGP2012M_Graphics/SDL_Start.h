#pragma once
#include "SDL.h"



class SDL_Start
{
public:



	bool windowOpen = true;
	bool fullScreen = false;

		//window aspect/width/height

	int Height = 540;
	int Width = 960;
	float AspectRatio;
	int left;
	int newHeight;
	int newWidth;

	//get desktop stuff
	int dH;
	int dW;
	SDL_DisplayMode DM;

	SDL_Window *win;
	SDL_GLContext glcontext;

	SDL_GLContext Init()
	{
		//SDL Initialise
		SDL_Init(SDL_INIT_EVERYTHING);

		//get desktop info
		SDL_GetCurrentDisplayMode(0, &DM);
		auto tWidth = DM.w;
		auto tHeight = DM.h;
		dW = tWidth / 2;
		dH = tHeight / 2;

		//SDL create window
		win = SDL_CreateWindow("Jacob Lord - 14587899- CGP2012M-1819", dW/2, dH/2, dW, dH, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

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
		glcontext = SDL_GL_CreateContext(win);
		AspectRatio = (float)Width / (float)Height;
		SDL_GetWindowSize(win, &Width, &Height);
		glViewport(0, 0, Width, Height);


		return glcontext;
	}
};