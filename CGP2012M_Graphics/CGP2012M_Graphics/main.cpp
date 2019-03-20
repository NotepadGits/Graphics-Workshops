#include "main.h"



int main(int argc, char *argv[])
{
	//Initialise SDL, make a window, blah
	SDL_Start sdl;
	sdl.Init();

	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	//Create our triangle
	Triangle_T tri;

	//create a square
	Square sq;

	//create test Circle
	Circle cr(0.2f);

	//create spawner
	Spawner spn;

	//We're gonna just count up the time every frame
	//so lets make a variable for that
	GLfloat currentTime;

	//Initialise the camera	
	Camera::INIT();

	//SDL_Event event;
	Events ev;
	ev.playerPassthrough(&tri);

	while (true)
	{
		//Clear the screen with a single colour
		glClearColor(1.0f, 0.8f, 1.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set the current time to the ticks in SDL
		currentTime = SDL_GetTicks();

		spn.spawn();

		//Actually draw the triangle on the screen



		//sq.setCurrentTime(currentTime);
		sq.render();


		spn.update();


		cr.update();
		cr.render();

		tri.render();

		//RenderPresent() -- swap/flip buffers
		SDL_GL_SwapWindow(sdl.win);


		//-------------


		//The event thing
		//SDL_Event event;

		//if (SDL_PollEvent(&event))
		//{
		//	//If they close the window break the loop
		//	if (event.type == SDL_QUIT)
		//		break;
		//}
		int temp = ev.HandleEvents(sdl);
		if (temp == 69)
		{
		break;
		}
	}

	//Clean up
	SDL_GL_DeleteContext(sdl.glcontext);
	SDL_Quit();

	return 0;
}