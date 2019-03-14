#pragma once
#include "SDL.h"

class SDL_Start
{
public:
	SDL_Window *win;
	SDL_GLContext glcontext;

	SDL_GLContext Init()
	{
		//SDL Initialise
		SDL_Init(SDL_INIT_EVERYTHING);

		//SDL create window
		win = SDL_CreateWindow("OpenGL Window", 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

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

		return glcontext;
	}
};