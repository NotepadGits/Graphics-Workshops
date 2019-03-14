#include "main.h"



int main(int argc, char *argv[]) 
{
	//SDL_Event event;

	//Initialise SDL, make a window, blah
	SDL_Start sdl;
	sdl.Init();
	
	//Create our triangle
	Triangle_T tri;

	//create a square
	Square sq;

	//test create circle
	Circle Cir(0.35f);

	//We're gonna just count up the time every frame
	//so lets make a variable for that
	GLfloat currentTime;

	Events ev();

	while (true)
	{		
		//Clear the screen with a single colour
		glClearColor(1.0f, 0.8f, 1.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT); 

		//Set the current time to the ticks in SDL
		currentTime = SDL_GetTicks();

		//Set the current time (in the shader)
		tri.setCurrentTime(currentTime);

		//Actually draw the triangle on the screen
		//tri.render();

		sq.setCurrentTime(currentTime);
		sq.render();
		Cir.render();

		//RenderPresent() -- swap/flip buffers
		SDL_GL_SwapWindow(sdl.win);

		//event handeling
		//ev.Update();


	}

	//Clean up
	SDL_GL_DeleteContext(sdl.glcontext);
	SDL_Quit();

	return 0;
}
