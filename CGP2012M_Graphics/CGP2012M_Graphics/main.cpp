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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	//Create our triangle
	Triangle_T tri;

	//create a square
	Square sq;
	Bomb bomb;
	BombText bText;
	Lives lives;
	//create spawner
	Spawner3D spn;

	//We're gonna just count up the time every frame
	//so lets make a variable for that
	GLfloat currentTime;

	//Initialise the camera	
	Camera::INIT(sdl.dW, sdl.dH);

	//SDL_Event event;
	Events3D ev;
	ev.playerPassthrough(&tri);

	Control3D control(&tri, &spn);

	while (true)
	{
		//Clear the screen with a single colour
		glClearColor(1.0f, 0.8f, 1.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set the current time to the ticks in SDL
		currentTime = SDL_GetTicks();

		spn.spawn();
		control.update();
		Camera::update();
		//Actually draw the triangle on the screen



		//sq.setCurrentTime(currentTime);
		sq.render();


		spn.update();


		tri.render(lives);
		control.redner();
		bomb.render();
		bText.render();
	lives.render();
		//RenderPresent() -- swap/flip buffers
		SDL_GL_SwapWindow(sdl.win);


		//-------------


		int temp = ev.HandleEvents(sdl, spn, control, bomb);
		if (lives.LivesRemaining < 1)
		{
		break;
		}
	}

	//Clean up
	SDL_GL_DeleteContext(sdl.glcontext);
	SDL_Quit();

	return 0;
}