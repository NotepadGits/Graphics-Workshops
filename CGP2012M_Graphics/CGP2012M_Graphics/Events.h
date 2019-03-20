#pragma once
#include "SDL.h"
#include "SDL_Start.h"
#include "Triangle_T.h"
#include <GL/glew.h>
#include "spawner.h"
#include "bulletController.h"
class Events
{
public:
	SDL_Event event;

	Triangle_T* player;
	Events()
	{

	}
	;
	void playerPassthrough(Triangle_T* plr)
	{
		player = plr;
	}
	/*Events(SDL_Event eventPassThrough)
	{
		event = eventPassThrough;
	};*/

	int HandleEvents(SDL_Start &sdl, Spawner &spn, Control &control)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				SDL_Quit();
				return 69;
			}
			if (event.type == SDL_KEYDOWN)
				switch (event.key.keysym.sym)
				{
				case SDLK_F11:
					if (sdl.fullScreen == true)
					{
						sdl.fullScreen = false;
						SDL_SetWindowFullscreen(sdl.win, SDL_WINDOW_RESIZABLE);
					}
					else {
						sdl.fullScreen = true;
						SDL_SetWindowFullscreen(sdl.win, SDL_WINDOW_FULLSCREEN_DESKTOP);
					}

					std::cout << "Window resized w:" << sdl.Width << " h:" << sdl.Height << std::endl;
					SDL_GetWindowSize(sdl.win, &sdl.Width, &sdl.Height);
					sdl.newWidth = sdl.Height * sdl.AspectRatio;
					sdl.left = (sdl.Width - sdl.newWidth) / 2;
					glViewport(sdl.left, 0, sdl.newWidth, sdl.Height);

					break;
				case SDLK_w:
					player->translate = glm::translate(player->translate, glm::vec3((float)cos(player->angle)*0.02f, (float)sin(player->angle)*0.02f, 0.0f));
					break;
				case SDLK_s:
					player->translate = glm::translate(player->translate, glm::vec3((float)cos(player->angle)*-0.02f, (float)sin(player->angle)*-0.02f, 0.0f));
					break;
				case SDLK_a:
					player->angle += glm::radians(10.0f);
					player->rotation = glm::rotate(player->rotation, glm::radians(10.0f), glm::vec3(0, 0, 1));
					break;
				case SDLK_d:
					player->angle -= glm::radians(10.0f);
					player->rotation = glm::rotate(player->rotation, glm::radians(-10.0f), glm::vec3(0, 0, 1));
					break;
				case SDLK_f:
					spn.bub.erase(spn.bub.begin(),spn.bub.end());
					break;
				case SDLK_SPACE:
					control.shoot();
					break;
				case SDLK_9:
					spn.bub.push_back(new Circle(0.2f));
					break;

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
