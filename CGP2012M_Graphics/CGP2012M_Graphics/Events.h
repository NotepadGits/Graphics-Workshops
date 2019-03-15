#pragma once
#include "SDL.h"
#include "SDL_Start.h"

class Events
{
public:
	SDL_Event event;

	Events()
	{

	}
	;

	/*Events(SDL_Event eventPassThrough)
	{
		event = eventPassThrough;
	};*/

	int HandleEvents(SDL_Start &sdl)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				SDL_Quit();
				return 69;
			}
			if (event.type == SDL_WINDOWEVENT)
			{
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					std::cout << "Window resized w:" << sdl.Width << " h:" << sdl.Height << std::endl;
					SDL_GetWindowSize(sdl.win, &sdl.Width, &sdl.Height);
					sdl.newWidth = sdl.Height * sdl.AspectRatio;
					sdl.left = (sdl.Width - sdl.newWidth) / 2;
					glViewport(sdl.left, 0, sdl.newWidth, sdl.Height);
					break;
				}

				return 0;
			}
		}
	};



};
