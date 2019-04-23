#pragma once
#include "SDL.h"
#include "SDL_Start.h"
#include "Triangle_T.h"
#include <GL/glew.h>
#include "spawner.h"
#include "bulletController.h"
#include "bomb.h"

class Light
{
public:


	glm::vec3 lightCol;
	glm::vec3 lightPosition;
	glm::vec3 viewPosition;



	Light()
	{

	};


};
