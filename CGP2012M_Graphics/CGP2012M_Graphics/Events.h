#pragma once
#include <GL/glew.h>
#include <array>
#include <cmath>
#include "SDL.h"


class Events
{
public:

	SDL_Event event;

	Events()
	{

	};

	void Update()
	{

		if (SDL_PollEvent(&event))
		{
			//If they close the window break the loop
			if (event.type == SDL_QUIT);

		}
	}

};



