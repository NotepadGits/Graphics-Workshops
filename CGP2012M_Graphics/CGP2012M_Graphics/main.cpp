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
	Triangle3D tri(0.1f, "..//..//Assets//Models//test.obj");

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

	Cube wallLeft("..//..//Assets//Models//blenderCube.obj");
	wallLeft.setScale(0.1, 1.4, 0.1);
	wallLeft.setTranslate(-0.5, 1, 0);
	Cube wallRight("..//..//Assets//Models//blenderCube.obj");
	wallRight.setScale(0.1, 1.4, 0.1);
	wallRight.setTranslate(3.5, 1, 0);
	Cube wallDown("..//..//Assets//Models//blenderCube.obj");
	wallDown.setScale(2.1, 0.1, 0.1);
	wallDown.setTranslate(1.5, -0.5, 0);
	Cube wallUp("..//..//Assets//Models//blenderCube.obj");
	wallUp.setScale(2.1, 0.1, 0.1);
	wallUp.setTranslate(1.5, 2.5, 0);

	SkyBox sky("..//..//Assets//Models//blenderCube.obj");
	sky.setScale(200,200,200);
	
	//Model testMod;
	//ModelImport importer;
	//importer.LoadOBJ2("..//..//Assets//Models//blenderSphere.obj", testMod.vertices, testMod.texCoords, testMod.normals, testMod.indices);
	//testMod.setBuffers();
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
		sky.render();
		sq.render();


		lives.render();
		wallLeft.render();
		wallRight.render();
		wallDown.render();
		wallUp.render();

		spn.update();


		tri.render(lives);
		control.redner();





		bomb.render();
		bText.render();

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