#pragma once
#include "SDL.h"

class Events
{
public:
	SDL_Event event;

	Events(SDL_Event eventPassThrough)
	{
		event = eventPassThrough;
	};

	void HandleEvents()
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				//	SDL_Quit();
			}
		}
	};



};
