#pragma once
#include "SDL.h"
#include "SDL_Start.h"
#include "Triangle_T.h"
#include <GL/glew.h>
#include "spawner.h"
#include "bulletController.h"
#include "bomb.h"
#include <vector>

class Light
{
public:
	
	static glm::vec3 lightCol1;
	static glm::vec3 lightPosition1;

	static glm::vec3 lightCol2;
	static glm::vec3 lightPosition2;

	static glm::vec3 lightCol3;
	static glm::vec3 lightPosition3;
	
	void moveLight(int x,  glm::vec3 pos)
	{
		if (x == 1) lightPosition1 = pos;
		if (x == 2) lightPosition2 = pos;
		if (x == 3) lightPosition3 = pos;
	}

	void setColour(int x, glm::vec3 col)
	{
		if (x == 1) lightPosition1 = col;
		if (x == 2) lightPosition2 = col;
		if (x == 3) lightPosition3 = col;
	}


};
glm::vec3 Light::lightCol1;
glm::vec3 Light::lightPosition1;

glm::vec3 Light::lightCol2;
glm::vec3 Light::lightPosition2;

glm::vec3 Light::lightCol3;
glm::vec3 Light::lightPosition3;